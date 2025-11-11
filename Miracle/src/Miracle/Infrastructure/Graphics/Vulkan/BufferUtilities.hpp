#pragma once

#include <tuple>

#include "Vulkan.hpp"
#include "Vma.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class BufferUtilities {
	public:
		BufferUtilities() = delete;

		static std::tuple<vk::Buffer, VmaAllocation, VmaAllocationInfo> createStagingBuffer(
			GraphicsContext& m_context,
			vk::DeviceSize bufferSize
		);

		static std::tuple<vk::Buffer, VmaAllocation, VmaAllocationInfo> createBuffer(
			GraphicsContext& m_context,
			vk::BufferUsageFlags usage,
			vk::DeviceSize bufferSize
		);

		static void copyBuffer(
			GraphicsContext& m_context,
			vk::Buffer destination,
			vk::Buffer source,
			vk::DeviceSize size
		);
	};
}
