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
			vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		),
		m_vertexCount(vertices.size())
	{
		auto bufferData = m_bufferMemory.mapMemory(0, m_size);
		std::memcpy(bufferData, vertices.data(), m_size);
		m_bufferMemory.unmapMemory();
	}

	void VertexBuffer::bind(const vk::raii::CommandBuffer& commandBuffer) const {
		commandBuffer.bindVertexBuffers(0, *m_buffer, static_cast<vk::DeviceSize>(0));
	}
}
