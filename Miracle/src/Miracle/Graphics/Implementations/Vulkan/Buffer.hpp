#pragma once

#include <variant>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Buffer {
	protected:
		const Device& m_device;

		vk::DeviceSize m_size;
		vk::raii::DeviceMemory m_memory = nullptr;
		vk::raii::Buffer m_buffer = nullptr;

	public:
		Buffer(
			const Device& device,
			vk::DeviceSize size,
			vk::BufferUsageFlags usage,
			vk::MemoryPropertyFlags properties
		);

		virtual ~Buffer() = default;

		inline const vk::raii::DeviceMemory& getAllocatedMemory() const { return m_memory; }

		inline const vk::raii::Buffer& getRawBuffer() const { return m_buffer; }

	private:
		std::variant<MiracleError, uint32_t> selectMemoryType(
			uint32_t filter,
			vk::MemoryPropertyFlags properties
		) const;
	};
}
