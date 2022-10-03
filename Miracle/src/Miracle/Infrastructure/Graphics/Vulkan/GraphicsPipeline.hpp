#pragma once

#include <Miracle/Application/Graphics/IGraphicsPipeline.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsPipeline : public Application::IGraphicsPipeline {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::raii::Pipeline m_pipeline = nullptr;

	public:
		GraphicsPipeline(
			Application::ILogger& logger,
			GraphicsContext& context
		);
	};
}
