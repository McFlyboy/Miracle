#include "Swapchain.hpp"

#include <limits>
#include <functional>
#include <algorithm>
#include <utility>
#include <array>

#include <Miracle/MiracleError.hpp>
#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	Swapchain::Swapchain(
		const Device& device,
		const Surface& surface
	) :
		m_device(device),
		m_surface(surface)
	{
		auto& supportDetails = m_device.getSupportDetails().swapchainSupportDetails;

		auto extent = selectExtent(supportDetails.capabilities);
		auto imageCount = selectImageCount(supportDetails.capabilities);
		auto format = selectFormat(supportDetails.formats);
		auto presentMode = selectPresentMode(supportDetails.presentModesSupported, false);

		auto& queueFamilyIndices = m_device.getSupportDetails().queueFamilyIndices;

		auto indices = std::array{
			queueFamilyIndices.graphicsFamilyIndex.value(),
			queueFamilyIndices.presentFamilyIndex.value()
		};

		bool isImagesShared = indices[0] != indices[1];

		auto result = m_device.createSwapchain({
			.flags                 = {},
			.surface               = *m_surface.getRawSurface(),
			.minImageCount         = imageCount,
			.imageFormat           = format.format,
			.imageColorSpace       = format.colorSpace,
			.imageExtent           = extent,
			.imageArrayLayers      = 1,
			.imageUsage            = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode      = isImagesShared ? vk::SharingMode::eConcurrent          : vk::SharingMode::eExclusive,
			.queueFamilyIndexCount = isImagesShared ? static_cast<uint32_t>(indices.size()) : 0,
			.pQueueFamilyIndices   = isImagesShared ? indices.data()                        : nullptr,
			.preTransform          = supportDetails.capabilities.currentTransform,
			.compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode           = presentMode,
			.clipped               = true,
			.oldSwapchain          = nullptr
		});

		if (result.index() == 0) {
			throw std::get<MiracleError>(result);
		}

		m_swapchain = std::move(std::get<vk::raii::SwapchainKHR>(result));

		m_imageFormat = format.format;
		m_imageExtent = extent;

		try {
			auto images = m_swapchain.getImages();
			m_images.reserve(images.size());

			for (auto& image : images) {
				m_images.emplace_back(image);
			}
		}
		catch (const std::exception& e) {
			Logger::error("Failed to retrieve images from Vulkan swapchain!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineSwapchainImagesRetrievalError;
		}

		m_imageViews.reserve(m_images.size());

		for (auto& image : m_images) {
			auto result = createViewForImage(image);

			if (result.index() == 0) {
				throw std::get<MiracleError>(result);
			}

			m_imageViews.push_back(std::move(std::get<vk::raii::ImageView>(result)));
		}

		m_renderPass = RenderPass(m_device, m_imageFormat);

		m_framebuffers.reserve(m_imageViews.size());

		for (auto& imageView : m_imageViews) {
			auto result = m_device.createFramebuffer({
				.flags           = {},
				.renderPass      = *m_renderPass.getRawRenderPass(),
				.attachmentCount = 1,
				.pAttachments    = &*imageView,
				.width           = m_imageExtent.width,
				.height          = m_imageExtent.height,
				.layers          = 1
			});

			if (result.index() == 0) {
				throw std::get<MiracleError>(result);
			}

			m_framebuffers.push_back(std::move(std::get<vk::raii::Framebuffer>(result)));
		}
	}

	vk::Extent2D Swapchain::selectExtent(
		const vk::SurfaceCapabilitiesKHR& capabilities
	) const {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}

		auto currentExtent = m_surface.getSurfaceTarget().getCurrentExtent();

		return vk::Extent2D{
			.width  = std::clamp(
				currentExtent.width,
				capabilities.minImageExtent.width,
				capabilities.maxImageExtent.width
			),
			.height = std::clamp(
				currentExtent.height,
				capabilities.minImageExtent.height,
				capabilities.maxImageExtent.height
			)
		};
	}

	uint32_t Swapchain::selectImageCount(
		const vk::SurfaceCapabilitiesKHR& capabilities
	) const {
		auto preferredImageCount = capabilities.minImageCount + 1;

		if (capabilities.maxImageCount == 0) {
			return preferredImageCount;
		}

		return std::min(preferredImageCount, capabilities.maxImageCount);
	}

	vk::SurfaceFormatKHR Swapchain::selectFormat(
		const std::vector<vk::SurfaceFormatKHR>& availableFormats
	) const {
		for (auto& format : availableFormats) {
			if (
				format.format == vk::Format::eB8G8R8A8Srgb
					&& format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear
			) {
				return format;
			}
		}

		return availableFormats.front();
	}

	vk::PresentModeKHR Swapchain::selectPresentMode(
		const PresentModesSupported& presentModesSupported,
		bool useVsync
	) const {
		return useVsync
			? presentModesSupported.mailboxModeSupported
				? vk::PresentModeKHR::eMailbox
				: vk::PresentModeKHR::eFifo
			: vk::PresentModeKHR::eImmediate;
	}

	std::variant<MiracleError, vk::raii::ImageView> Swapchain::createViewForImage(
		const vk::Image& image
	) const {
		return m_device.createImageView({
			.flags            = {},
			.image            = image,
			.viewType         = vk::ImageViewType::e2D,
			.format           = m_imageFormat,
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
				.layerCount     = 1,
			}
		});
	}
}
