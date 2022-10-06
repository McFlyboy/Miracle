#include "VertexBuffer.hpp"

#include <exception>
#include <vector>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	VertexBuffer::VertexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context
	) :
		m_logger(logger),
		m_context(context)
	{
		auto vertexData = std::vector{
			-0.5f,  0.5f,
			 0.5f,  0.5f,
			 0.0f, -0.5f
		};

		try {
			m_buffer = m_context.getDevice().createBuffer(
				vk::BufferCreateInfo{
					.flags                 = {},
					.size                  = static_cast<vk::DeviceSize>(sizeof(vertexData.front()) * vertexData.size()),
					.usage                 = vk::BufferUsageFlagBits::eVertexBuffer,
					.sharingMode           = vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = 0,
					.pQueueFamilyIndices   = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan vertex buffer.\n{}", e.what()));
			throw Application::VertexBufferErrors::CreationError();
		}

		auto allocationCreateInfo = VmaAllocationCreateInfo{
            .flags			= {},
            .usage			= {},
            .requiredFlags	= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            .preferredFlags	= {},
            .memoryTypeBits	= {},
            .pool			= {},
            .pUserData		= {},
            .priority		= {}
		};

		auto allocationCreateResult = vmaAllocateMemoryForBuffer(
			m_context.getAllocator(),
			*m_buffer,
			&allocationCreateInfo,
			&m_allocation,
			nullptr
		);

		if (allocationCreateResult != VK_SUCCESS) {
			m_logger.error("Failed to allocate memory for Vulkan vertex buffer");
			throw Application::VertexBufferErrors::AllocationError();
		}

		auto bindResult = vmaBindBufferMemory(m_context.getAllocator(), m_allocation, *m_buffer);

		if (bindResult != VK_SUCCESS) {
			m_logger.error("Failed to bind allocated memory to Vulkan vertex buffer");
			vmaFreeMemory(m_context.getAllocator(), m_allocation);
			throw Application::VertexBufferErrors::BindError();
		}

		void* data = nullptr;

		auto mapResult = vmaMapMemory(m_context.getAllocator(), m_allocation, &data);

		if (mapResult != VK_SUCCESS) {
			m_logger.error("Failed to map allocated device memory to CPU accessible memory");
			vmaFreeMemory(m_context.getAllocator(), m_allocation);
			throw Application::VertexBufferErrors::MapError();
		}

		std::memcpy(data, vertexData.data(), sizeof(vertexData.front()) * vertexData.size());

		vmaUnmapMemory(m_context.getAllocator(), m_allocation);

		m_vertexCount = vertexData.size() / 2;

		m_logger.info("Vulkan vertex buffer created");
	}

	VertexBuffer::~VertexBuffer() {
		m_logger.info("Destroying Vulkan vertex buffer...");

		vmaFreeMemory(m_context.getAllocator(), m_allocation);
	}

	void VertexBuffer::bind() {
		m_context.getCommandBuffer().bindVertexBuffers(0, *m_buffer, {0});
	}
}
