#pragma once

#include <cstdint>

#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Queue {
	protected:
		vk::raii::Queue m_queue;

		Queue() : m_queue(nullptr) {}

		Queue(
			const vk::raii::Device& device,
			uint32_t queueFamilyIndex
		) :
			m_queue(device.getQueue(queueFamilyIndex, 0))
		{}

	public:
		virtual ~Queue() = default;

		inline void waitIdle() const { m_queue.waitIdle(); }
	};
}
