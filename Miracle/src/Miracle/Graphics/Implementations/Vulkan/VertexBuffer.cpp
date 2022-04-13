#include "VertexBuffer.hpp"

#include <cstring>
#include <utility>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	VertexBuffer::VertexBuffer(
		const Device& device,
		const std::vector<Vertex>& vertices
	) :
		m_device(device)
	{
		auto verticesMemorySize = sizeof(Vertex) * vertices.size();

		auto bufferCreateResult = m_device.createBuffer({
			.flags                 = {},
			.size                  = verticesMemorySize,
			.usage                 = vk::BufferUsageFlagBits::eVertexBuffer,
			.sharingMode           = vk::SharingMode::eExclusive,
			.queueFamilyIndexCount = {},
			.pQueueFamilyIndices   = {}
		});

		if (bufferCreateResult.index() == 0) {
			throw std::get<MiracleError>(bufferCreateResult);
		}

		m_buffer = std::move(std::get<vk::raii::Buffer>(bufferCreateResult));

		auto memoryRequirements = m_buffer.getMemoryRequirements();

		auto memorySelectResult = selectMemoryType(
			memoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		if (memorySelectResult.index() == 0) {
			throw std::get<MiracleError>(memorySelectResult);
		}

		auto& selectedMemoryType = std::get<uint32_t>(memorySelectResult);

		auto memoryAllocateResult = m_device.allocateMemory({
			.allocationSize  = memoryRequirements.size,
			.memoryTypeIndex = selectedMemoryType
		});

		if (memoryAllocateResult.index() == 0) {
			throw std::get<MiracleError>(memoryAllocateResult);
		}

		m_bufferMemory = std::move(std::get<vk::raii::DeviceMemory>(memoryAllocateResult));

		m_buffer.bindMemory(*m_bufferMemory, 0);

		auto bufferData = m_bufferMemory.mapMemory(0, verticesMemorySize);
		std::memcpy(bufferData, vertices.data(), verticesMemorySize);
		m_bufferMemory.unmapMemory();

		m_vertexCount = vertices.size();
	}

	void VertexBuffer::bind(const vk::raii::CommandBuffer& commandBuffer) const {
		commandBuffer.bindVertexBuffers(0, *m_buffer, static_cast<vk::DeviceSize>(0));
	}

	std::variant<MiracleError, uint32_t> VertexBuffer::selectMemoryType(
		uint32_t filter,
		vk::MemoryPropertyFlags properties
	) const {
		auto& deviceMemoryProperties = m_device.getSupportDetails().memoryProperties;

		for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++) {
			if (
				(filter & (1 << i))
					&& (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties
			) {
				return i;
			}
		}

		Logger::error("Failed to select suitable memory type for Vulkan vertex buffer!");
		return MiracleError::VulkanGraphicsEngineVertexBufferMemoryTypeSelectionError;
	}
}
