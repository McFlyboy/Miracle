#pragma once

#include <utility>

#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsQueue : public Queue {
	public:
		GraphicsQueue() = default;

		GraphicsQueue(vk::raii::Queue&& queue) : Queue(std::move(queue)) {}

		GraphicsQueue& operator=(GraphicsQueue&& right) {
			m_queue = std::move(right.m_queue);
			return *this;
		}
	};
}
