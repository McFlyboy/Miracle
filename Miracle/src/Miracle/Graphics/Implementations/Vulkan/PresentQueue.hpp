#pragma once

#include <utility>
#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class PresentQueue : public Queue {
	public:
		PresentQueue() = default;

		PresentQueue(
			const vk::raii::Device& device,
			uint32_t queueFamilyIndex
		);

		PresentQueue& operator=(PresentQueue&& right) {
			m_queue = std::move(right.m_queue);
			return *this;
		}

		std::optional<MiracleError> present(
			const vk::raii::Semaphore& waitSemaphore,
			const vk::raii::SwapchainKHR& swapchain,
			uint32_t imageIndex
		) const;
	};
}
