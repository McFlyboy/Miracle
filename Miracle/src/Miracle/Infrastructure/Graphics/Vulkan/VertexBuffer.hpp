#pragma once

#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/Graphics/IVertexBuffer.hpp>
#include "Vulkan.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class VertexBuffer : public Application::IVertexBuffer {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::raii::Buffer m_buffer = nullptr;

	public:
		VertexBuffer(
			Application::ILogger& logger,
			GraphicsContext& context
		);
	};
}
