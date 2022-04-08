#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"
#include "Surface.hpp"
#include "RenderPass.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Swapchain {
	private:
		const Device& m_device;
		const Surface& m_surface;

		vk::Extent2D m_imageExtent;
		uint32_t m_imageCount;
		vk::SurfaceFormatKHR m_surfaceFormat;
		vk::raii::SwapchainKHR m_swapchain = nullptr;
		std::vector<vk::Image> m_images;
		std::vector<vk::raii::ImageView> m_imageViews;
		RenderPass m_renderPass;
		std::vector<vk::raii::Framebuffer> m_framebuffers;
		bool m_outdated = false;

	public:
		Swapchain(
			const Device& device,
			const Surface& surface
		);

		void executeRenderPass(
			const vk::raii::CommandBuffer& commandBuffer,
			uint32_t imageIndex,
			const vk::ClearColorValue& clearColor,
			std::function<void ()> commands
		);

		std::variant<MiracleError, uint32_t> acquireNextImage(
			const vk::raii::Semaphore& signalSemaphore
		) const;

		std::optional<MiracleError> recreate();

		inline const vk::raii::SwapchainKHR& getRawSwapchain() const { return m_swapchain; }

		inline const vk::Extent2D& getImageExtent() const { return m_imageExtent; }

		inline const RenderPass& getRenderPass() const { return m_renderPass; }

		inline bool isOutdated() const { return m_outdated; }

		inline void setOutdated(bool outdated) { m_outdated = outdated; }

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

		std::variant<MiracleError, vk::raii::SwapchainKHR> createSwapchain() const;

		std::optional<MiracleError> fillImageList();

		std::optional<MiracleError> fillImageViewList();

		std::variant<MiracleError, vk::raii::ImageView> createViewForImage(
			const vk::Image& image
		) const;

		std::optional<MiracleError> fillFramebufferList();
	};
}
