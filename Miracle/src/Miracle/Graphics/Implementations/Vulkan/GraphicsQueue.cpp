#include "GraphicsQueue.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsQueue::GraphicsQueue(
		const vk::raii::Device& device,
		uint32_t queueFamilyIndex
	) :
		Queue(device, queueFamilyIndex)
	{
		try {
			m_commandPool = device.createCommandPool({
				.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
				.queueFamilyIndex = queueFamilyIndex
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan command pool!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineCommandPoolCreationError;
		}

		try {
			auto commandBuffers = device.allocateCommandBuffers({
				.commandPool        = *m_commandPool,
				.level              = vk::CommandBufferLevel::ePrimary,
				.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size())
			});

			for (int i = 0; i < m_commandBuffers.size(); i++) {
				m_commandBuffers[i] = std::move(commandBuffers[i]);
			}
		}
		catch (const std::exception& e) {
			Logger::error("Failed to allocate Vulkan command buffers!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineCommandBufferAllocationError;
		}
	}

	std::optional<MiracleError> GraphicsQueue::recordCommands(
		int bufferIndex,
		std::function<void (const vk::raii::CommandBuffer&)> recording
	) const {
		try {
			m_commandBuffers[bufferIndex].reset();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to reset Vulkan command buffer!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandBufferResetError;
		}

		try {
			m_commandBuffers[bufferIndex].begin({
				.flags            = {},
				.pInheritanceInfo = {}
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to begin Vulkan command recording!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandRecordBeginError;
		}

		recording(m_commandBuffers[bufferIndex]);

		try {
			m_commandBuffers[bufferIndex].end();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to end Vulkan command recording!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandRecordEndError;
		}

		return std::nullopt;
	}

	std::optional<MiracleError> GraphicsQueue::submitRecorded(
		int bufferIndex,
		const vk::raii::Semaphore& waitSemaphore,
		const vk::raii::Semaphore& signalSemaphore,
		const vk::raii::Fence& signalFence
	) const {
		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		try {
			m_queue.submit(
				vk::SubmitInfo{
					.waitSemaphoreCount = 1,
					.pWaitSemaphores = &*waitSemaphore,
					.pWaitDstStageMask = &waitStage,
					.commandBufferCount = 1,
					.pCommandBuffers = &*m_commandBuffers[bufferIndex],
					.signalSemaphoreCount = 1,
					.pSignalSemaphores = &*signalSemaphore
				},
				*signalFence
			);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to submit Vulkan command buffer to graphics queue!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandBufferSubmissionError;
		}

		return std::nullopt;
	}

	std::optional<MiracleError> GraphicsQueue::submitRecordedWithoutSync(int bufferIndex) const {
		try {
			m_queue.submit(
				vk::SubmitInfo{
					.waitSemaphoreCount   = 0,
					.pWaitSemaphores      = nullptr,
					.pWaitDstStageMask    = nullptr,
					.commandBufferCount   = 1,
					.pCommandBuffers      = &*m_commandBuffers[bufferIndex],
					.signalSemaphoreCount = 0,
					.pSignalSemaphores    = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to submit Vulkan command buffer to graphics queue!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandBufferSubmissionError;
		}

		return std::nullopt;
	}
}
