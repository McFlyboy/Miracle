#pragma once

#include <vector>

#include "Vulkan.hpp"
#include "Device.hpp"
#include "Buffer.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class IndexBuffer : public Buffer {
	private:
		const uint32_t m_indexCount;

	public:
		IndexBuffer(
			const Device& device,
			const std::vector<uint32_t>& indices
		);

		void bind(const vk::raii::CommandBuffer& commandBuffer) const;

		inline uint32_t getIndexCount() const { return m_indexCount; }
	};
}
