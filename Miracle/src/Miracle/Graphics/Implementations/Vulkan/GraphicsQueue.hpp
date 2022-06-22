#pragma once

#include <utility>
#include <functional>
#include <optional>
#include <array>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Queue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsQueue : public Queue {
	private:
		vk::raii::CommandPool m_commandPool = nullptr;
		std::array<vk::raii::CommandBuffer, 2> m_commandBuffers = { nullptr, nullptr };

	public:
		GraphicsQueue() = default;

		GraphicsQueue(
			const vk::raii::Device& device,
			uint32_t queueFamilyIndex
		);

		GraphicsQueue& operator=(GraphicsQueue&& right) noexcept {
			m_queue = std::move(right.m_queue);
			m_commandPool = std::move(right.m_commandPool);
			m_commandBuffers = std::move(right.m_commandBuffers);
			return *this;
		}

		std::optional<MiracleError> recordCommands(
			int bufferIndex,
			std::function<void (const vk::raii::CommandBuffer&)> recording
		) const;

		std::optional<MiracleError> submitRecorded(
			int bufferIndex,
			const vk::raii::Semaphore& waitSemaphore,
			const vk::raii::Semaphore& signalSemaphore,
			const vk::raii::Fence& signalFence
		) const;

		std::optional<MiracleError> submitRecordedWithoutSync(int bufferIndex) const;
	};
}
