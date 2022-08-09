#include "Swapchain.hpp"

#include <exception>
#include <algorithm>
#include <limits>
#include <array>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	Swapchain::Swapchain(
		Application::ILogger& logger,
		GraphicsContext& context,
		const Application::SwapchainInitProps& initProps
	) :
		m_logger(logger),
		m_context(context),
		m_preferredImageCount(selectImageCount()),
		m_surfaceFormat(selectSurfaceFormat(initProps.useSrgb))
	{
		auto& deviceInfo = m_context.getDeviceInfo();

		bool sharingModeEnabled = deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value()
			!= deviceInfo.queueFamilyIndices.presentFamilyIndex.value();

		auto queueFamilyIndices = std::array{
			deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
			deviceInfo.queueFamilyIndices.presentFamilyIndex.value()
		};

		try {
			m_swapchain = m_context.getDevice().createSwapchainKHR(
				vk::SwapchainCreateInfoKHR{
					.flags                 = {},
					.surface               = *m_context.getSurface(),
					.minImageCount         = m_preferredImageCount,
					.imageFormat           = m_surfaceFormat.format,
					.imageColorSpace       = m_surfaceFormat.colorSpace,
					.imageExtent           = selectExtent(),
					.imageArrayLayers      = 1,
					.imageUsage            = vk::ImageUsageFlagBits::eColorAttachment,
					.imageSharingMode      = sharingModeEnabled
						? vk::SharingMode::eConcurrent
						: vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = sharingModeEnabled
						? static_cast<uint32_t>(queueFamilyIndices.size())
						: 0,
					.pQueueFamilyIndices   = sharingModeEnabled
						? queueFamilyIndices.data()
						: nullptr,
					.preTransform          = m_context.getCurrentSurfaceTransformation(),
					.compositeAlpha        = vk::CompositeAlphaFlagBitsKHR::eOpaque,
					.presentMode           = selectPresentMode(initProps.useVsync),
					.clipped               = true,
					.oldSwapchain          = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan swapchain.\n{}", e.what()));
			throw Application::SwapchainErrors::CreationError();
		}

		m_logger.info("Vulkan swapchain created");
	}

	Swapchain::~Swapchain() {
		m_logger.info("Destroying Vulkan swapchain...");
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
}
