#pragma once

#include <vector>
#include <variant>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"
#include "Surface.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Swapchain {
	private:
		const Device& m_device;
		const Surface& m_surface;

		vk::raii::SwapchainKHR m_swapchain = nullptr;
		vk::Format m_imageFormat;
		vk::Extent2D m_imageExtent;
		std::vector<vk::Image> m_images;
		std::vector<vk::raii::ImageView> m_imageViews;

	public:
		Swapchain(
			const Device& device,
			const Surface& surface
		);

	private:
		vk::Extent2D selectExtent(
			const vk::SurfaceCapabilitiesKHR& capabilities
		) const;

		uint32_t selectImageCount(
			const vk::SurfaceCapabilitiesKHR& capabilities
		) const;

		vk::SurfaceFormatKHR selectFormat(
			const std::vector<vk::SurfaceFormatKHR>& availableFormats
		) const;

		vk::PresentModeKHR selectPresentMode(
			const PresentModesSupported& presentModesSupported,
			bool useVsync
		) const;

		std::variant<MiracleError, vk::raii::ImageView> createViewForImage(
			const vk::Image& image
		) const;
	};
}
