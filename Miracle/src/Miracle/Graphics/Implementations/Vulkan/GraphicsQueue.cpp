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
				.commandBufferCount = 1
			});

			m_commandBuffer = std::move(commandBuffers.front());
		}
		catch (const std::exception& e) {
			Logger::error("Failed to allocate Vulkan command buffer!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineCommandBufferAllocationError;
		}
	}

	std::optional<MiracleError> GraphicsQueue::recordCommands(std::function<void ()> recording) {
		try {
			m_commandBuffer.reset();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to reset Vulkan command buffer!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandBufferResetError;
		}

		try {
			m_commandBuffer.begin({
				.flags            = {},
				.pInheritanceInfo = {}
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to begin Vulkan command recording!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandRecordBeginError;
		}

		recording();

		try {
			m_commandBuffer.end();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to end Vulkan command recording!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineCommandRecordEndError;
		}

		std::nullopt;
	}
}
