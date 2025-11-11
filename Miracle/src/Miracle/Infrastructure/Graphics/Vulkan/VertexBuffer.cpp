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

		auto [stagingBuffer, stagingAllocation, stagingAllocationInfo] =
			std::tuple<vk::Buffer, VmaAllocation, VmaAllocationInfo>();

		try {
			auto [buffer, allocation, allocationInfo] = BufferUtilities::createStagingBuffer(
				m_context,
				requiredBufferSize
			);

			stagingBuffer = buffer;
			stagingAllocation = allocation;
			stagingAllocationInfo = allocationInfo;
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create staging buffer for Vulkan vertex buffer.\n{}", e.what())
			);

			throw Application::VertexBufferErrors::CreationError();
		}

		auto stagingBufferData = stagingAllocationInfo.pMappedData;
		std::memcpy(stagingBufferData, vertices.data(), requiredBufferSize);

		try {
			auto [buffer, allocation, allocationInfo] = BufferUtilities::createBuffer(
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

			vmaDestroyBuffer(m_context.getAllocator(), stagingBuffer, stagingAllocation);
			throw Application::VertexBufferErrors::CreationError();
		}

		BufferUtilities::copyBuffer(m_context, m_buffer, stagingBuffer, requiredBufferSize);

		vmaDestroyBuffer(m_context.getAllocator(), stagingBuffer, stagingAllocation);

		m_vertexCount = vertices.size();

		m_logger.info("Vulkan vertex buffer created");
	}

	VertexBuffer::~VertexBuffer() {
		m_logger.info("Destroying Vulkan vertex buffer...");

		vmaDestroyBuffer(m_context.getAllocator(), m_buffer, m_allocation);
	}

	void VertexBuffer::bind() {
		m_context.getGraphicsCommandBuffer().bindVertexBuffers(0, m_buffer, {0});
	}
}
