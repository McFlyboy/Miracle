#include "Buffer.hpp"

#include <utility>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	Buffer::Buffer(
		const Device& device,
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		vk::MemoryPropertyFlags properties
	) :
		m_device(device),
		m_size(size)
	{
		auto bufferCreateResult = m_device.createBuffer({
			.flags                 = {},
			.size                  = m_size,
			.usage                 = usage,
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
			properties
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
	}

	std::variant<MiracleError, uint32_t> Buffer::selectMemoryType(
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

		Logger::error("Failed to select suitable memory type for Vulkan buffer!");
		return MiracleError::VulkanGraphicsEngineBufferMemoryTypeSelectionError;
	}
}
