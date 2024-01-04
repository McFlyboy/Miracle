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
		m_minimumImageCount(selectMinimumImageCount(initProps.useTripleBuffering)),
		m_surfaceFormat(selectSurfaceFormat()),
		m_imageExtent(selectExtent()),
		m_presentMode(selectPresentMode(initProps.useVsync)),
		m_swapchain(createSwapchain())
	{
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

		m_logger.info(
			std::format(
				"Vulkan swapchain created with {} images and {} present mode",
				m_images.size(),
				vk::to_string(m_presentMode)
			)
		);
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

		m_context.getGraphicsCommandBuffer().beginRenderPass(
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
		m_context.getGraphicsCommandBuffer().endRenderPass();
	}

	void Swapchain::swap() {
		auto result = m_context.getPresentQueue().presentKHR(
			vk::PresentInfoKHR{
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &*m_context.getGraphicsCommandExecutionCompletedSemaphore(),
				.swapchainCount     = 1,
				.pSwapchains        = &*m_swapchain,
				.pImageIndices      = &m_imageIndex,
				.pResults           = nullptr
			}
		);

		if (result == vk::Result::eSuboptimalKHR) [[unlikely]] {
			m_logger.warning("Swapped Vulkan swapchain in suboptimal state");
		}

		m_context.nextGraphicsCommandBuffer();

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

		m_context.recreatePresentCompletedSemaphores();

		m_imageIndex = getNextImageIndex();

		m_logger.info(
			std::format(
				"Vulkan swapchain re-created with {} images and {} present mode",
				m_images.size(),
				vk::to_string(m_presentMode)
			)
		);
	}

	void Swapchain::setVsync(bool useVsync) {
		m_presentMode = selectPresentMode(useVsync);
	}

	void Swapchain::setTripleBuffering(bool useTripleBuffering) {
		m_minimumImageCount = selectMinimumImageCount(useTripleBuffering);
		m_presentMode = selectPresentMode(isUsingVsync());
	}

	uint32_t Swapchain::selectMinimumImageCount(bool useTripleBuffering) const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		if (useTripleBuffering) {
			if (swapchainSupport.hasTripleBufferingSupport) return 3;

			m_logger.warning("Triple buffering not supported. Falling back to double buffering");
		}

		return 2;
	}

	vk::SurfaceFormatKHR Swapchain::selectSurfaceFormat() const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		for (auto& surfaceFormat : swapchainSupport.surfaceFormats) {
			if (
				surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear
					&& (
						surfaceFormat.format == vk::Format::eR8G8B8A8Srgb
							|| surfaceFormat.format == vk::Format::eB8G8R8A8Srgb
							|| surfaceFormat.format == vk::Format::eA8B8G8R8SrgbPack32
					)
			) {
				return surfaceFormat;
			}
		}

		m_logger.warning(
			"Preferred surface formats for swapchain not available.\nFallback solution might not give intended visual results!"
		);

		return swapchainSupport.surfaceFormats.front();
	}

	vk::Extent2D Swapchain::selectExtent() const {
		auto currentExtent = m_context.getCurrentSurfaceExtent();
		
		if (std::holds_alternative<vk::Extent2D>(currentExtent.extent)) {
			return std::get<vk::Extent2D>(currentExtent.extent);
		}

		auto currentTargetExtent = m_context.getTarget().getCurrentVulkanExtent();

		auto& extentBounds = std::get<SurfaceExtentBounds>(currentExtent.extent);

		return vk::Extent2D{
			.width  = std::clamp(
				currentTargetExtent.width,
				extentBounds.minExtent.width,
				extentBounds.maxExtent.width
			),
			.height = std::clamp(
				currentTargetExtent.height,
				extentBounds.minExtent.height,
				extentBounds.maxExtent.height
			)
		};
	}

	vk::PresentModeKHR Swapchain::selectPresentMode(bool useVsync) const {
		auto& swapchainSupport = m_context.getDeviceInfo().extensionSupport.swapchainSupport.value();

		return useVsync
			? swapchainSupport.hasMailboxModePresentationSupport && m_minimumImageCount > 2
				? vk::PresentModeKHR::eMailbox
				: vk::PresentModeKHR::eFifo
			: vk::PresentModeKHR::eImmediate;
	}

	vk::raii::SwapchainKHR Swapchain::createSwapchain() const {
		auto& queueFamilyIndices = m_context.getDeviceInfo().queueFamilyIndices;

		bool useSharingMode = queueFamilyIndices.graphicsFamilyIndex.value()
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
					.minImageCount = m_minimumImageCount,
					.imageFormat = m_surfaceFormat.format,
					.imageColorSpace = m_surfaceFormat.colorSpace,
					.imageExtent = m_imageExtent,
					.imageArrayLayers = 1,
					.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
					.imageSharingMode = useSharingMode
						? vk::SharingMode::eConcurrent
						: vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = useSharingMode
						? static_cast<uint32_t>(queueFamilyIndexArray.size())
						: 0,
					.pQueueFamilyIndices = useSharingMode
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
			*m_context.getGraphicsCommandPresentCompletedSemaphore()
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
