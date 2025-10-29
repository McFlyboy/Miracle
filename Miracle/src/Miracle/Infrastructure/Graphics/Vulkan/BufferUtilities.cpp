#include "BufferUtilities.hpp"

#include <array>
#include <stdexcept>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	std::tuple<vk::Buffer, VmaAllocation, VmaAllocationInfo> BufferUtilities::createStagingBuffer(
		GraphicsContext& m_context,
		vk::DeviceSize bufferSize
	) {
		auto createInfo = VkBufferCreateInfo{
			.sType				   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext				   = {},
			.flags                 = {},
			.size                  = bufferSize,
			.usage                 = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode           = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices   = nullptr
		};

		auto allocationCreateInfo = VmaAllocationCreateInfo{
			.flags			= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
				| VMA_ALLOCATION_CREATE_MAPPED_BIT,
			.usage			= VMA_MEMORY_USAGE_AUTO,
			.requiredFlags	= {},
			.preferredFlags	= {},
			.memoryTypeBits	= {},
			.pool			= nullptr,
			.pUserData		= nullptr,
			.priority		= 1.0f
		};

		VkBuffer buffer;
		VmaAllocation allocation;
		VmaAllocationInfo allocationInfo;

		auto result = vmaCreateBuffer(
			m_context.getAllocator(),
			&createInfo,
			&allocationCreateInfo,
			&buffer,
			&allocation,
			&allocationInfo
		);

		if (result != VK_SUCCESS)
			throw std::runtime_error("vmaCreateBuffer");

		return { buffer, allocation, allocationInfo };
	}

	std::tuple<vk::Buffer, VmaAllocation, VmaAllocationInfo> BufferUtilities::createBuffer(
		GraphicsContext& m_context,
		vk::BufferUsageFlags usage,
		vk::DeviceSize bufferSize
	) {
		auto& deviceInfo = m_context.getDeviceInfo();

		bool useSharingMode = deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value()
			!= deviceInfo.queueFamilyIndices.transferFamilyIndex.value();

		auto queueFamilyIndexArray = std::array{
			deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
			deviceInfo.queueFamilyIndices.transferFamilyIndex.value()
		};

		auto createInfo = VkBufferCreateInfo{
			.sType				   = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.pNext				   = {},
			.flags                 = {},
			.size                  = bufferSize,
			.usage                 = static_cast<VkBufferUsageFlags>(usage) | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode           = useSharingMode
				? VK_SHARING_MODE_CONCURRENT
				: VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = useSharingMode
				? static_cast<uint32_t>(queueFamilyIndexArray.size())
				: 0,
			.pQueueFamilyIndices   = useSharingMode
				? queueFamilyIndexArray.data()
				: nullptr
		};

		auto allocationCreateInfo = VmaAllocationCreateInfo{
			.flags          = {},
			.usage          = VMA_MEMORY_USAGE_AUTO,
			.requiredFlags  = {},
			.preferredFlags = {},
			.memoryTypeBits = {},
			.pool           = nullptr,
			.pUserData      = nullptr,
			.priority       = 1.0f
		};

		VkBuffer buffer;
		VmaAllocation allocation;
		VmaAllocationInfo allocationInfo;

		auto result = vmaCreateBuffer(
			m_context.getAllocator(),
			&createInfo,
			&allocationCreateInfo,
			&buffer,
			&allocation,
			&allocationInfo
		);

		if (result != VK_SUCCESS)
			throw std::runtime_error("vmaCreateBuffer");

		return { buffer, allocation, allocationInfo };
	}

	void BufferUtilities::copyBuffer(
		GraphicsContext& m_context,
		vk::Buffer destination,
		vk::Buffer source,
		vk::DeviceSize size
	) {
		m_context.recordTransferCommands(
			[&]() {
				m_context.getTransferCommandBuffer().copyBuffer(
					source,
					destination,
					vk::BufferCopy{
						.srcOffset = 0,
						.dstOffset = 0,
						.size = size
					}
				);
			}
		);

		m_context.submitTransferRecording();
		m_context.waitForDeviceIdle();
	}
}
