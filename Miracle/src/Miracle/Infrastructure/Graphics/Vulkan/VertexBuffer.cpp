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

		auto requiredBufferSize = static_cast<vk::DeviceSize>(sizeof(vertices.front()) * vertices.size());

		auto [stagingBuffer, stagingAllocation] = createStagingBuffer(requiredBufferSize);

		auto stagingBufferData = m_context.getAllocator().getAllocationInfo(stagingAllocation).pMappedData;
		std::memcpy(stagingBufferData, vertices.data(), requiredBufferSize);

		try {
			auto [buffer, allocation] = createBuffer(requiredBufferSize);
			m_buffer = buffer;
			m_allocation = allocation;
		}
		catch (const std::exception& e) {
			m_context.getAllocator().destroyBuffer(stagingBuffer, stagingAllocation);
			throw e;
		}

		copyBuffer(m_buffer, stagingBuffer, requiredBufferSize);

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

	std::pair<vk::Buffer, vma::Allocation> VertexBuffer::createStagingBuffer(vk::DeviceSize bufferSize) const {
		try {
			return m_context.getAllocator().createBuffer(
				vk::BufferCreateInfo{
					.flags                 = {},
					.size                  = bufferSize,
					.usage                 = vk::BufferUsageFlagBits::eTransferSrc,
					.sharingMode           = vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = 0,
					.pQueueFamilyIndices   = nullptr
				},
				vma::AllocationCreateInfo{
					.flags			= vma::AllocationCreateFlagBits::eHostAccessSequentialWrite
						| vma::AllocationCreateFlagBits::eMapped,
					.usage			= vma::MemoryUsage::eAuto,
					.requiredFlags	= {},
					.preferredFlags	= {},
					.memoryTypeBits	= {},
					.pool			= nullptr,
					.pUserData		= nullptr,
					.priority		= 1.0f
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format("Failed to create staging buffer for Vulkan vertex buffer.\n{}", e.what())
			);

			throw Application::VertexBufferErrors::CreationError();
		}
	}

	std::pair<vk::Buffer, vma::Allocation> VertexBuffer::createBuffer(vk::DeviceSize bufferSize) const {
		try {
			return m_context.getAllocator().createBuffer(
				vk::BufferCreateInfo{
					.flags                 = {},
					.size                  = bufferSize,
					.usage                 = vk::BufferUsageFlagBits::eVertexBuffer
						| vk::BufferUsageFlagBits::eTransferDst,
					.sharingMode           = vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = 0,
					.pQueueFamilyIndices   = nullptr
				},
				vma::AllocationCreateInfo{
					.flags          = vma::AllocationCreateFlagBits::eDedicatedMemory,
					.usage          = vma::MemoryUsage::eAuto,
					.requiredFlags  = {},
					.preferredFlags = {},
					.memoryTypeBits = {},
					.pool           = nullptr,
					.pUserData      = nullptr,
					.priority       = 1.0f
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format("Failed to create Vulkan vertex buffer.\n{}", e.what())
			);

			throw Application::VertexBufferErrors::CreationError();
		}
	}

	void VertexBuffer::copyBuffer(vk::Buffer destination, vk::Buffer source, vk::DeviceSize size) const {
		m_context.getCommandBuffer().reset();
		m_context.getCommandBuffer().begin(
			vk::CommandBufferBeginInfo{
				.flags            = {},
				.pInheritanceInfo = {}
			}
		);

		m_context.getCommandBuffer().copyBuffer(
			source,
			destination,
			vk::BufferCopy{
				.srcOffset = 0,
				.dstOffset = 0,
				.size      = size
			}
		);

		m_context.getCommandBuffer().end();
		m_context.getGraphicsQueue().submit(
			vk::SubmitInfo{
				.waitSemaphoreCount   = 0,
				.pWaitSemaphores      = nullptr,
				.pWaitDstStageMask    = nullptr,
				.commandBufferCount   = 1,
				.pCommandBuffers      = &*m_context.getCommandBuffer(),
				.signalSemaphoreCount = 0,
				.pSignalSemaphores    = nullptr
			}
		);

		m_context.getGraphicsQueue().waitIdle();
	}
}
