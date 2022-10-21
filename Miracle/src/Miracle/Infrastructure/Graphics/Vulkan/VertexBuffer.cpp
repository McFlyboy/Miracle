#include "VertexBuffer.hpp"

#include <exception>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	VertexBuffer::VertexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context,
		const std::vector<Vector2f>& vertices
	) :
		m_logger(logger),
		m_context(context)
	{
		if (vertices.empty()) {
			m_logger.error("No vertices provided for Vulkan vertex buffer creation");
			throw Application::VertexBufferErrors::NoVerticesProvidedError();
		}

		try {
			auto [buffer, allocation] = m_context.getAllocator().createBuffer(
				vk::BufferCreateInfo{
					.flags                 = {},
					.size                  = static_cast<vk::DeviceSize>(sizeof(vertices.front()) * vertices.size()),
					.usage                 = vk::BufferUsageFlagBits::eVertexBuffer,
					.sharingMode           = vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = 0,
					.pQueueFamilyIndices   = nullptr
				},
				vma::AllocationCreateInfo{
					.flags			= vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
					.usage			= vma::MemoryUsage::eAuto,
					.requiredFlags	= {},
					.preferredFlags	= {},
					.memoryTypeBits	= {},
					.pool			= {},
					.pUserData		= {},
					.priority		= {}
				}
			);

			m_buffer = buffer;
			m_allocation = allocation;
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan vertex buffer.\n{}", e.what()));
			throw Application::VertexBufferErrors::CreationError();
		}

		void* data = nullptr;

		try {
			data = m_context.getAllocator().mapMemory(m_allocation);
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format(
					"Failed to map allocated device memory to CPU accessible memory.\n{}",
					e.what()
				)
			);

			m_context.getAllocator().destroyBuffer(m_buffer, m_allocation);
			throw Application::VertexBufferErrors::MapError();
		}

		std::memcpy(data, vertices.data(), sizeof(vertices.front()) * vertices.size());

		m_context.getAllocator().unmapMemory(m_allocation);

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
