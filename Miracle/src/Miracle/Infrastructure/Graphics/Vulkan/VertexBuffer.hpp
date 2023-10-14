#pragma once

#include <vector>

#include <Miracle/Common/Models/Vertex.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/Graphics/IVertexBuffer.hpp>
#include "Vulkan.hpp"
#include "Vma.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class VertexBuffer : public Application::IVertexBuffer {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::Buffer m_buffer = nullptr;
		vma::Allocation m_allocation = nullptr;
		uint32_t m_vertexCount = 0;

	public:
		VertexBuffer(
			Application::ILogger& logger,
			GraphicsContext& context,
			const std::vector<Vertex>& vertices
		);

		~VertexBuffer();

		virtual uint32_t getVertexCount() const override { return m_vertexCount; }

		virtual void bind() override;
	};
}
