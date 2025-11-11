#include "IndexBuffer.hpp"

#include <exception>
#include <format>

#include "BufferUtilities.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	IndexBuffer::IndexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context,
		const std::vector<Face>& faces
	) :
		m_logger(logger),
		m_context(context)
	{
		if (faces.empty()) {
			m_logger.error("No indices provided for Vulkan index buffer creation");
			throw Application::IndexBufferErrors::NoIndicesProvidedError();
		}

		auto requiredBufferSize = static_cast<vk::DeviceSize>(sizeof(faces.front()) * faces.size());

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
				std::format("Failed to create staging buffer for Vulkan index buffer.\n{}", e.what())
			);

			throw Application::IndexBufferErrors::CreationError();
		}

		auto stagingBufferData = stagingAllocationInfo.pMappedData;
		std::memcpy(stagingBufferData, faces.data(), requiredBufferSize);

		try {
			auto [buffer, allocation, allocationInfo] = BufferUtilities::createBuffer(
				m_context,
				vk::BufferUsageFlagBits::eIndexBuffer,
				requiredBufferSize
			);

			m_buffer = buffer;
			m_allocation = allocation;
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan index buffer.\n{}", e.what())
			);

			vmaDestroyBuffer(m_context.getAllocator(), stagingBuffer, stagingAllocation);
			throw Application::IndexBufferErrors::CreationError();
		}

		BufferUtilities::copyBuffer(m_context, m_buffer, stagingBuffer, requiredBufferSize);

		vmaDestroyBuffer(m_context.getAllocator(), stagingBuffer, stagingAllocation);

		m_indexCount = faces.front().indices.size() * faces.size();

		m_logger.info("Vulkan index buffer created");
	}

	IndexBuffer::~IndexBuffer() {
		m_logger.info("Destroying Vulkan index buffer...");

		vmaDestroyBuffer(m_context.getAllocator(), m_buffer, m_allocation);
	}

	void IndexBuffer::bind() {
		m_context.getGraphicsCommandBuffer().bindIndexBuffer(m_buffer, 0, vk::IndexType::eUint32);
	}
}
