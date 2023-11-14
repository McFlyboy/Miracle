#include "Swapchain.hpp"

#include <exception>
#include <algorithm>
#include <limits>
#include <array>
#include <format>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	Swapchain::Swapchain(
		Application::ILogger& logger,
		GraphicsContext& context,
		const Application::SwapchainInitProps& initProps
	) :
		m_logger(logger),
		m_context(context),
		m_preferredImageCount(selectImageCount()),
		m_surfaceFormat(selectSurfaceFormat(initProps.useSrgb)),
		m_imageExtent(selectExtent()),
		m_presentMode(selectPresentMode(initProps.useVsync))
	{
		m_swapchain = createSwapchain();

		auto images = m_swapchain.getImages();

		m_images.reserve(images.size());

		for (auto& image : images) {
			m_images.emplace_back(image, createImageView(image));
		}

		m_renderPass = createRenderPass();

		m_frameBuffers.reserve(m_images.size());

		for (auto& [image, imageView] : m_images) {
			m_frameBuffers.push_back(createFrameBuffer(imageView));
		}

		m_imageIndex = getNextImageIndex();

		m_logger.info("Vulkan swapchain created");
	}

	Swapchain::~Swapchain() {
		m_logger.info("Destroying Vulkan swapchain...");
	}

	Application::SwapchainImageSize Swapchain::getImageSize() const {
		return Application::SwapchainImageSize{
			.width  = m_imageExtent.width,
			.height = m_imageExtent.height
		};
	}

	void Swapchain::beginRenderPass(ColorRgb clearColor) {
		auto clearValues = std::array{
			vk::ClearValue(
				vk::ClearColorValue(
					std::array{
						clearColor.redChannel,
						clearColor.greenChannel,
						clearColor.blueChannel,
						1.0f
					}
				)
			)
		};

		m_context.getCommandBuffer().beginRenderPass(
			vk::RenderPassBeginInfo{
				.renderPass      = *m_renderPass,
				.framebuffer     = *m_frameBuffers[m_imageIndex],
				.renderArea      = vk::Rect2D{
					.offset = vk::Offset2D{
						.x = 0,
						.y = 0
					},
					.extent = m_imageExtent
				},
				.clearValueCount = static_cast<uint32_t>(clearValues.size()),
				.pClearValues    = clearValues.data()
			},
			vk::SubpassContents::eInline
		);
	}

	void Swapchain::endRenderPass() {
		m_context.getCommandBuffer().endRenderPass();
	}

	void Swapchain::swap() {
		auto result = m_context.getPresentQueue().presentKHR(
			vk::PresentInfoKHR{
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &*m_context.getCommandExecutionSignalSemaphore(),
				.swapchainCount     = 1,
				.pSwapchains        = &*m_swapchain,
				.pImageIndices      = &m_imageIndex,
				.pResults           = nullptr
			}
		);

		if (result == vk::Result::eSuboptimalKHR) [[unlikely]] {
			m_logger.warning("Swapped Vulkan swapchain in suboptimal state");
		}

		m_context.nextCommandBuffer();

		m_imageIndex = getNextImageIndex();
	}

	void Swapchain::recreate() {
		m_frameBuffers.clear();
		m_images.clear();
		m_swapchain.clear();

		m_imageExtent = selectExtent();
		m_swapchain = createSwapchain();

		auto images = m_swapchain.getImages();

		for (auto& image : images) {
			m_images.emplace_back(image, createImageView(image));
		}

		for (auto& [image, imageView] : m_images) {
			m_frameBuffers.push_back(createFrameBuffer(imageView));
		}

		m_context.recreateSemaphores();

		m_imageIndex = getNextImageIndex();

		m_logger.info("Vulkan swapchain re-created");
	}

	uint32_t Swapchain::selectImageCount() const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		auto preferredImageCount = swapchainSupport.minImageCount + 1;

		return swapchainSupport.maxImageCount.has_value()
			? std::min(preferredImageCount, swapchainSupport.maxImageCount.value())
			: preferredImageCount;
	}

	vk::SurfaceFormatKHR Swapchain::selectSurfaceFormat(bool useSrgb) const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		for (auto& surfaceFormat : swapchainSupport.surfaceFormats) {
			switch (surfaceFormat.format) {
			case vk::Format::eB8G8R8A8Srgb:
			case vk::Format::eR8G8B8A8Srgb:
				if (useSrgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
					return surfaceFormat;
				}

				break;

			case vk::Format::eB8G8R8A8Unorm:
			case vk::Format::eR8G8B8A8Unorm:
				if (!useSrgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
					return surfaceFormat;
				}

				break;

			default:
				continue;
			}
		}

		m_logger.warning(
			"Common Vulkan surface formats for swapchain not available.\nFallback solution might not give intended result!"
		);

		return swapchainSupport.surfaceFormats.front();
	}

	vk::Extent2D Swapchain::selectExtent() const {
		auto currentExtent = m_context.getCurrentSurfaceExtent();
		
		if (currentExtent.extent.has_value()) {
			return currentExtent.extent.value();
		}

		auto currentTargetExtent = m_context.getTarget().getCurrentVulkanExtent();

		return vk::Extent2D{
			.width  = std::clamp(
				currentTargetExtent.width,
				currentExtent.minExtent.width,
				currentExtent.maxExtent.width
			),
			.height = std::clamp(
				currentTargetExtent.height,
				currentExtent.minExtent.height,
				currentExtent.maxExtent.height
			)
		};
	}

	vk::PresentModeKHR Swapchain::selectPresentMode(bool useVsync) const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		return useVsync
			? swapchainSupport.hasMailboxModePresentationSupport
				? vk::PresentModeKHR::eMailbox
				: vk::PresentModeKHR::eFifo
			: vk::PresentModeKHR::eImmediate;
	}

	vk::raii::SwapchainKHR Swapchain::createSwapchain() const {
		auto& queueFamilyIndices = m_context.getDeviceInfo().queueFamilyIndices;

		bool sharingModeEnabled = queueFamilyIndices.graphicsFamilyIndex.value()
			!= queueFamilyIndices.presentFamilyIndex.value();

		auto queueFamilyIndexArray = std::array{
			queueFamilyIndices.graphicsFamilyIndex.value(),
			queueFamilyIndices.presentFamilyIndex.value()
		};

		try {
			return m_context.getDevice().createSwapchainKHR(
				vk::SwapchainCreateInfoKHR{
					.flags = {},
					.surface = *m_context.getSurface(),
					.minImageCount = m_preferredImageCount,
					.imageFormat = m_surfaceFormat.format,
					.imageColorSpace = m_surfaceFormat.colorSpace,
					.imageExtent = m_imageExtent,
					.imageArrayLayers = 1,
					.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
					.imageSharingMode = sharingModeEnabled
						? vk::SharingMode::eConcurrent
						: vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = sharingModeEnabled
						? static_cast<uint32_t>(queueFamilyIndexArray.size())
						: 0,
					.pQueueFamilyIndices = sharingModeEnabled
						? queueFamilyIndexArray.data()
						: nullptr,
					.preTransform = m_context.getCurrentSurfaceTransformation(),
					.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
					.presentMode = m_presentMode,
					.clipped = true,
					.oldSwapchain = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(std::format("Failed to create Vulkan swapchain.\n{}", e.what()));
			throw Application::SwapchainErrors::CreationError();
		}
	}

	vk::raii::ImageView Swapchain::createImageView(const vk::Image& image) const {
		try {
			return m_context.getDevice().createImageView(
				vk::ImageViewCreateInfo{
					.flags            = {},
					.image            = image,
					.viewType         = vk::ImageViewType::e2D,
					.format           = m_surfaceFormat.format,
					.components       = vk::ComponentMapping{
						.r = vk::ComponentSwizzle::eIdentity,
						.g = vk::ComponentSwizzle::eIdentity,
						.b = vk::ComponentSwizzle::eIdentity,
						.a = vk::ComponentSwizzle::eIdentity
					},
					.subresourceRange = vk::ImageSubresourceRange{
						.aspectMask     = vk::ImageAspectFlagBits::eColor,
						.baseMipLevel   = 0,
						.levelCount     = 1,
						.baseArrayLayer = 0,
						.layerCount     = 1
					}
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format(
					"Failed to create Vulkan image view for image in swapchain.\n{}",
					e.what()
				)
			);

			throw Application::SwapchainErrors::CreationError();
		}
	}

	vk::raii::RenderPass Swapchain::createRenderPass() const {
		auto attachments = std::array{
			vk::AttachmentDescription{
				.flags          = {},
				.format         = m_surfaceFormat.format,
				.samples        = vk::SampleCountFlagBits::e1,
				.loadOp         = vk::AttachmentLoadOp::eClear,
				.storeOp        = vk::AttachmentStoreOp::eStore,
				.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare,
				.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
				.initialLayout  = vk::ImageLayout::eUndefined,
				.finalLayout    = vk::ImageLayout::ePresentSrcKHR
			}
		};

		auto attachmentReferences = std::array{
			vk::AttachmentReference{
				.attachment = 0,
				.layout     = vk::ImageLayout::eColorAttachmentOptimal
			}
		};

		auto subpasses = std::array{
			vk::SubpassDescription{
				.flags                   = {},
				.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics,
				.inputAttachmentCount    = 0,
				.pInputAttachments       = nullptr,
				.colorAttachmentCount    = static_cast<uint32_t>(attachmentReferences.size()),
				.pColorAttachments       = attachmentReferences.data(),
				.pResolveAttachments     = nullptr,
				.pDepthStencilAttachment = nullptr,
				.preserveAttachmentCount = 0,
				.pPreserveAttachments    = nullptr
			}
		};

		auto subpassDependencies = std::array{
			vk::SubpassDependency{
				.srcSubpass      = VK_SUBPASS_EXTERNAL,
				.dstSubpass      = 0,
				.srcStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput,
				.dstStageMask    = vk::PipelineStageFlagBits::eColorAttachmentOutput,
				.srcAccessMask   = vk::AccessFlags(),
				.dstAccessMask   = vk::AccessFlagBits::eColorAttachmentWrite,
				.dependencyFlags = {}
			}
		};

		try {
			return m_context.getDevice().createRenderPass(
				vk::RenderPassCreateInfo{
					.flags           = {},
					.attachmentCount = static_cast<uint32_t>(attachments.size()),
					.pAttachments    = attachments.data(),
					.subpassCount    = static_cast<uint32_t>(subpasses.size()),
					.pSubpasses      = subpasses.data(),
					.dependencyCount = static_cast<uint32_t>(subpassDependencies.size()),
					.pDependencies   = subpassDependencies.data()
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan render pass for swapchain.\n{}", e.what())
			);

			throw Application::SwapchainErrors::CreationError();
		}
	}

	vk::raii::Framebuffer Swapchain::createFrameBuffer(const vk::raii::ImageView& imageView) const {
		try {
			return m_context.getDevice().createFramebuffer(
				vk::FramebufferCreateInfo{
					.flags           = {},
					.renderPass      = *m_renderPass,
					.attachmentCount = 1,
					.pAttachments    = &*imageView,
					.width           = m_imageExtent.width,
					.height          = m_imageExtent.height,
					.layers          = 1
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan frame buffer for swapchain.\n{}", e.what())
			);

			throw Application::SwapchainErrors::CreationError();
		}
	}

	uint32_t Swapchain::getNextImageIndex() {
		auto [result, imageIndex] = m_swapchain.acquireNextImage(
			std::numeric_limits<uint64_t>().max(),
			*m_context.getCommandExecutionWaitSemaphore()
		);

		switch (result) {
		case vk::Result::eTimeout:
			m_logger.warning("Timed out on acquiring next Vulkan swapchain image");
			break;

		case vk::Result::eNotReady:
			m_logger.warning("No Vulkan image in swapchain was ready for use");
			break;

		case vk::Result::eSuboptimalKHR:
			m_logger.warning("Acquired Vulkan swapchain in suboptimal state");
			break;

		default: [[likely]]
			break;
		}		 
		
		return imageIndex;
	}
}
