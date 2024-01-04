#pragma once

#include <utility>
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

		uint32_t m_minimumImageCount;
		const vk::SurfaceFormatKHR m_surfaceFormat;
		vk::Extent2D m_imageExtent;
		vk::PresentModeKHR m_presentMode;
		vk::raii::SwapchainKHR m_swapchain;
		std::vector<std::pair<vk::Image, vk::raii::ImageView>> m_images;
		vk::raii::RenderPass m_renderPass = nullptr;
		std::vector<vk::raii::Framebuffer> m_frameBuffers;
		uint32_t m_imageIndex = 0;

	public:
		Swapchain(
			Application::ILogger& logger,
			GraphicsContext& context,
			const Application::SwapchainInitProps& initProps
		);

		~Swapchain();

		virtual Application::SwapchainImageSize getImageSize() const override;

		virtual void beginRenderPass(ColorRgb clearColor) override;

		virtual void endRenderPass() override;

		virtual void swap() override;

		virtual void recreate() override;

		virtual bool isUsingVsync() const override {
			return m_presentMode != vk::PresentModeKHR::eImmediate;
		}

		virtual void setVsync(bool useVsync) override;

		virtual bool isUsingTripleBuffering() const override {
			return m_minimumImageCount > 2;
		}

		virtual void setTripleBuffering(bool useTripleBuffering) override;

		const vk::raii::RenderPass& getRenderPass() const { return m_renderPass; }

	private:
		uint32_t selectMinimumImageCount(bool useTripleBuffering) const;

		vk::SurfaceFormatKHR selectSurfaceFormat() const;

		vk::Extent2D selectExtent() const;

		vk::PresentModeKHR selectPresentMode(bool useVsync) const;

		vk::raii::SwapchainKHR createSwapchain() const;

		vk::raii::ImageView createImageView(const vk::Image& image) const;

		vk::raii::RenderPass createRenderPass() const;

		vk::raii::Framebuffer createFrameBuffer(const vk::raii::ImageView& imageView) const;

		uint32_t getNextImageIndex();
	};
}
