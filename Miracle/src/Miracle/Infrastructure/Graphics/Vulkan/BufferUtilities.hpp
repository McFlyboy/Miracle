#pragma once

#include <utility>

#include "Vulkan.hpp"
#include "Vma.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class BufferUtilities {
	public:
		BufferUtilities() = delete;

		static std::pair<vk::Buffer, vma::Allocation> createStagingBuffer(
			GraphicsContext& m_context,
			vk::DeviceSize bufferSize
		);

		static std::pair<vk::Buffer, vma::Allocation> createBuffer(
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
