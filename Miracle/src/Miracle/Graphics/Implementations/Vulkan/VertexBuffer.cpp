#include "VertexBuffer.hpp"

#include <cstring>

namespace Miracle::Graphics::Implementations::Vulkan {
	VertexBuffer::VertexBuffer(
		const Device& device,
		const std::vector<Vertex>& vertices
	) :
		Buffer(
			device,
			sizeof(Vertex) * vertices.size(),
			vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		),
		m_vertexCount(vertices.size())
	{
		auto stagingBuffer = Buffer(
			m_device,
			m_size,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		auto stagingBufferData = stagingBuffer.getAllocatedMemory().mapMemory(0, m_size);
		std::memcpy(stagingBufferData, vertices.data(), m_size);
		stagingBuffer.getAllocatedMemory().unmapMemory();

		m_device.getGraphicsQueue()
			.recordCommands(
				0,
				[this, &stagingBuffer](const vk::raii::CommandBuffer& commandBuffer) {
					commandBuffer.copyBuffer(
						*stagingBuffer.getRawBuffer(),
						*m_buffer,
						vk::BufferCopy{
							.srcOffset = 0,
							.dstOffset = 0,
							.size      = m_size
						}
					);
				}
			);

		m_device.getGraphicsQueue()
			.submitRecordedWithoutSync(0);

		m_device.getGraphicsQueue()
			.waitIdle();
	}

	void VertexBuffer::bind(const vk::raii::CommandBuffer& commandBuffer) const {
		commandBuffer.bindVertexBuffers(0, *m_buffer, static_cast<vk::DeviceSize>(0));
	}
}
