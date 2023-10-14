#pragma once

#include <cstddef>
#include <vector>

#include <Miracle/Application/Graphics/IGraphicsPipeline.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "Vulkan.hpp"
#include "GraphicsContext.hpp"
#include "Swapchain.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsPipeline : public Application::IGraphicsPipeline {
	private:
		Application::ILogger& m_logger;
		Application::IFileAccess& m_fileAccess;
		GraphicsContext& m_context;
		Swapchain& m_swapchain;

		vk::raii::PipelineLayout m_layout = nullptr;
		vk::raii::Pipeline m_pipeline = nullptr;

	public:
		GraphicsPipeline(
			Application::ILogger& logger,
			Application::IFileAccess& fileAccess,
			GraphicsContext& context,
			Swapchain& swapchain
		);

		~GraphicsPipeline();

		virtual void bind() override;

		virtual void pushConstants(const Application::PushConstants& constants) override;

	private:
		vk::raii::ShaderModule createShaderModule(const std::vector<std::byte>& bytecode) const;
	};
}
