#include "VertexBuffer.hpp"

#include <exception>
#include <format>

#include "BufferUtilities.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	VertexBuffer::VertexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context,
		const std::vector<Vertex>& vertices
	) :
		m_logger(logger),
		m_context(context)
	{
		if (vertices.empty()) {
			m_logger.error("No vertices provided for Vulkan vertex buffer creation");
			throw Application::VertexBufferErrors::NoVerticesProvidedError();
		}

		auto requiredBufferSize = static_cast<vk::DeviceSize>(sizeof(vertices.front()) * vertices.size());

		auto [stagingBuffer, stagingAllocation] = std::pair<vk::Buffer, vma::Allocation>();

		try {
			auto [buffer, allocation] = BufferUtilities::createStagingBuffer(
				m_context,
				requiredBufferSize
			);

			stagingBuffer = buffer;
			stagingAllocation = allocation;
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create staging buffer for Vulkan vertex buffer.\n{}", e.what())
			);

			throw Application::VertexBufferErrors::CreationError();
		}

		auto stagingBufferData = m_context.getAllocator().getAllocationInfo(stagingAllocation).pMappedData;
		std::memcpy(stagingBufferData, vertices.data(), requiredBufferSize);

		try {
			auto [buffer, allocation] = BufferUtilities::createBuffer(
				m_context,
				vk::BufferUsageFlagBits::eVertexBuffer,
				requiredBufferSize
			);

			m_buffer = buffer;
			m_allocation = allocation;
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan vertex buffer.\n{}", e.what())
			);

			m_context.getAllocator().destroyBuffer(stagingBuffer, stagingAllocation);
			throw Application::VertexBufferErrors::CreationError();
		}

		BufferUtilities::copyBuffer(m_context, m_buffer, stagingBuffer, requiredBufferSize);

		m_context.getAllocator().destroyBuffer(stagingBuffer, stagingAllocation);

		m_vertexCount = vertices.size();

		m_logger.info("Vulkan vertex buffer created");
	}

	VertexBuffer::~VertexBuffer() {
		m_logger.info("Destroying Vulkan vertex buffer...");

		m_context.getAllocator().destroyBuffer(m_buffer, m_allocation);
	}

	void VertexBuffer::bind() {
		m_context.getCommandBuffer().bindVertexBuffers(0, m_buffer, {0});
	}
}
