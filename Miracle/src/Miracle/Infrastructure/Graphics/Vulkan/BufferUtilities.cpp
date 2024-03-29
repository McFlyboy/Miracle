#include "BufferUtilities.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	std::pair<vk::Buffer, vma::Allocation> BufferUtilities::createStagingBuffer(
		GraphicsContext& m_context,
		vk::DeviceSize bufferSize
	) {
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

	std::pair<vk::Buffer, vma::Allocation> BufferUtilities::createBuffer(
		GraphicsContext& m_context,
		vk::BufferUsageFlags usage,
		vk::DeviceSize bufferSize
	) {
		return m_context.getAllocator().createBuffer(
			vk::BufferCreateInfo{
				.flags                 = {},
				.size                  = bufferSize,
				.usage                 = usage | vk::BufferUsageFlagBits::eTransferDst,
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
