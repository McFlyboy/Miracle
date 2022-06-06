#include "IndexBuffer.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	IndexBuffer::IndexBuffer(
		const Device& device,
		const std::vector<uint32_t>& indices
	) :
		Buffer(
			device,
			sizeof(uint32_t) * indices.size(),
			vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
			vk::MemoryPropertyFlagBits::eDeviceLocal
		),
		m_indexCount(static_cast<uint32_t>(indices.size()))
	{
		auto stagingBuffer = Buffer(
			m_device,
			m_size,
			vk::BufferUsageFlagBits::eTransferSrc,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		auto stagingBufferData = stagingBuffer.getAllocatedMemory().mapMemory(0, m_size);
		std::memcpy(stagingBufferData, indices.data(), m_size);
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

	void IndexBuffer::bind(const vk::raii::CommandBuffer& commandBuffer) const {
		commandBuffer.bindIndexBuffer(*m_buffer, static_cast<vk::DeviceSize>(0), vk::IndexType::eUint32);
	}
}
