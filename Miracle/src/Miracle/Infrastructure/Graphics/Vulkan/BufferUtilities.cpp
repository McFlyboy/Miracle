#include "BufferUtilities.hpp"

#include <array>

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
		auto& deviceInfo = m_context.getDeviceInfo();

		bool useSharingMode = deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value()
			!= deviceInfo.queueFamilyIndices.transferFamilyIndex.value();

		auto queueFamilyIndexArray = std::array{
			deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
			deviceInfo.queueFamilyIndices.transferFamilyIndex.value()
		};

		return m_context.getAllocator().createBuffer(
			vk::BufferCreateInfo{
				.flags                 = {},
				.size                  = bufferSize,
				.usage                 = usage | vk::BufferUsageFlagBits::eTransferDst,
				.sharingMode           = useSharingMode
					? vk::SharingMode::eConcurrent
					: vk::SharingMode::eExclusive,
				.queueFamilyIndexCount = useSharingMode
					? static_cast<uint32_t>(queueFamilyIndexArray.size())
					: 0,
				.pQueueFamilyIndices   = useSharingMode
					? queueFamilyIndexArray.data()
					: nullptr
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
