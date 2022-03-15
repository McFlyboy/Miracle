#pragma once

#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Queue {
	protected:
		vk::raii::Queue m_queue;

		Queue() : m_queue(nullptr) {}

		Queue(vk::raii::Queue&& queue) : m_queue(std::move(queue)) {}

		virtual ~Queue() = default;
	};
}
