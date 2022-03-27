#pragma once

#include <utility>
#include <functional>
#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsQueue : public Queue {
	private:
		vk::raii::CommandPool m_commandPool = nullptr;
		vk::raii::CommandBuffer m_commandBuffer = nullptr;

	public:
		GraphicsQueue() = default;

		GraphicsQueue(
			const vk::raii::Device& device,
			uint32_t queueFamilyIndex
		);

		GraphicsQueue& operator=(GraphicsQueue&& right) {
			m_queue = std::move(right.m_queue);
			m_commandPool = std::move(right.m_commandPool);
			m_commandBuffer = std::move(right.m_commandBuffer);
			return *this;
		}

		std::optional<MiracleError> recordCommands(
			std::function<void (const vk::raii::CommandBuffer&)> recording
		) const;

		std::optional<MiracleError> submitRecorded(
			const vk::raii::Semaphore& waitSemaphore,
			const vk::raii::Semaphore& signalSemaphore,
			const vk::raii::Fence& signalFence
		) const;
	};
}
