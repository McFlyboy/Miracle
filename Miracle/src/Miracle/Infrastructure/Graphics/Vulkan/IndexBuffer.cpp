#include "IndexBuffer.hpp"

#include <exception>

#include <fmt/format.h>

#include "BufferUtilities.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	IndexBuffer::IndexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context,
		std::vector<uint32_t> indices
	) :
		m_logger(logger),
		m_context(context)
	{
		if (indices.empty()) {
			m_logger.error("No indices provided for Vulkan index buffer creation");
			throw Application::IndexBufferErrors::NoIndicesProvidedError();
		}

		auto requiredBufferSize = static_cast<vk::DeviceSize>(sizeof(indices.front()) * indices.size());

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
				fmt::format("Failed to create staging buffer for Vulkan index buffer.\n{}", e.what())
			);

			throw Application::IndexBufferErrors::CreationError();
		}

		auto stagingBufferData = m_context.getAllocator().getAllocationInfo(stagingAllocation).pMappedData;
		std::memcpy(stagingBufferData, indices.data(), requiredBufferSize);

		try {
			auto [buffer, allocation] = BufferUtilities::createBuffer(
				m_context,
				vk::BufferUsageFlagBits::eIndexBuffer,
				requiredBufferSize
			);

			m_buffer = buffer;
			m_allocation = allocation;
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format("Failed to create Vulkan index buffer.\n{}", e.what())
			);

			m_context.getAllocator().destroyBuffer(stagingBuffer, stagingAllocation);
			throw Application::IndexBufferErrors::CreationError();
		}

		BufferUtilities::copyBuffer(m_context, m_buffer, stagingBuffer, requiredBufferSize);

		m_context.getAllocator().destroyBuffer(stagingBuffer, stagingAllocation);

		m_indexCount = indices.size();

		m_logger.info("Vulkan index buffer created");
	}

	IndexBuffer::~IndexBuffer() {
		m_logger.info("Destroying Vulkan index buffer...");

		m_context.getAllocator().destroyBuffer(m_buffer, m_allocation);
	}

	void IndexBuffer::bind() {
		m_context.getCommandBuffer().bindIndexBuffer(m_buffer, 0, vk::IndexType::eUint32);
	}
}
