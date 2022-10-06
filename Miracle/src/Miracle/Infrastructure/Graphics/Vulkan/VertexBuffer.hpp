#pragma once

#include <vk_mem_alloc.h>

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
		VmaAllocation m_allocation = nullptr;
		uint32_t m_vertexCount = 0;

	public:
		VertexBuffer(
			Application::ILogger& logger,
			GraphicsContext& context
		);

		~VertexBuffer();

		inline virtual uint32_t getVertexCount() const override { return m_vertexCount; }

		virtual void bind() override;
	};
}
