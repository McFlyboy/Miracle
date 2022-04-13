#pragma once

#include <vector>
#include <variant>

#include <Miracle/MiracleError.hpp>
#include <Miracle/Graphics/Vertex.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class VertexBuffer {
	private:
		const Device& m_device;

		vk::raii::DeviceMemory m_bufferMemory = nullptr;
		vk::raii::Buffer m_buffer = nullptr;
		uint32_t m_vertexCount = 0;

	public:
		VertexBuffer(
			const Device& device,
			const std::vector<Vertex>& vertices
		);

		void bind(const vk::raii::CommandBuffer& commandBuffer) const;

		inline uint32_t getVertexCount() const { return m_vertexCount; }

	private:
		std::variant<MiracleError, uint32_t> selectMemoryType(
			uint32_t filter,
			vk::MemoryPropertyFlags properties
		) const;
	};
}
