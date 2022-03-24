#pragma once

#include <utility>

#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class PresentQueue : public Queue {
	public:
		PresentQueue() = default;

		PresentQueue(
			const vk::raii::Device& device,
			uint32_t queueFamilyIndex
		) :
			Queue(device, queueFamilyIndex)
		{}

		PresentQueue& operator=(PresentQueue&& right) {
			m_queue = std::move(right.m_queue);
			return *this;
		}
	};
}
