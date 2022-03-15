#pragma once

#include <utility>

#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class PresentQueue : public Queue {
	public:
		PresentQueue() = default;

		PresentQueue(vk::raii::Queue&& queue) : Queue(std::move(queue)) {}

		PresentQueue& operator=(PresentQueue&& right) {
			m_queue = std::move(right.m_queue);
			return *this;
		}
	};
}
