#pragma once

#include <Miracle/Application/Graphics/IRenderPass.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class RenderPass : public Application::IRenderPass {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;
		Swapchain& m_swapchain;

		vk::raii::RenderPass m_renderPass = nullptr;

	public:
		RenderPass(
			Application::ILogger& logger,
			GraphicsContext& context,
			Swapchain& swapchain
		);

		~RenderPass();
	};
}
