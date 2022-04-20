#pragma once

#include <vector>

#include <Miracle/Graphics/Vertex.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"
#include "Buffer.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class VertexBuffer : public Buffer {
	private:
		const uint32_t m_vertexCount;

	public:
		VertexBuffer(
			const Device& device,
			const std::vector<Vertex>& vertices
		);

		void bind(const vk::raii::CommandBuffer& commandBuffer) const;

		inline uint32_t getVertexCount() const { return m_vertexCount; }
	};
}
