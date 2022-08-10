#pragma once

#include <vector>

#include <Miracle/Application/Graphics/ISwapchain.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class Swapchain : public Application::ISwapchain {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::raii::SwapchainKHR m_swapchain = nullptr;
		std::vector<vk::Image> m_images;
		std::vector<vk::raii::ImageView> m_imageViews;
		uint32_t m_preferredImageCount;
		vk::SurfaceFormatKHR m_surfaceFormat;
		vk::Extent2D m_imageExtent;
		vk::PresentModeKHR m_presentMode;

	public:
		Swapchain(
			Application::ILogger& logger,
			GraphicsContext& context,
			const Application::SwapchainInitProps& initProps
		);

		~Swapchain();

	private:
		uint32_t selectImageCount() const;

		vk::SurfaceFormatKHR selectSurfaceFormat(bool useSrgb) const;

		vk::Extent2D selectExtent() const;

		vk::PresentModeKHR selectPresentMode(bool useVsync) const;

		vk::raii::ImageView createImageView(const vk::Image& image) const;
	};
}
