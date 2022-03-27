#include "PresentQueue.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	PresentQueue::PresentQueue(
		const vk::raii::Device& device,
		uint32_t queueFamilyIndex
	) : Queue(device, queueFamilyIndex) {}

	std::optional<MiracleError> PresentQueue::present(
		const vk::raii::Semaphore& waitSemaphore,
		const vk::raii::SwapchainKHR& swapchain,
		uint32_t imageIndex
	) const {
		try {
			m_queue.presentKHR({
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &*waitSemaphore,
				.swapchainCount     = 1,
				.pSwapchains        = &*swapchain,
				.pImageIndices      = &imageIndex,
				.pResults           = nullptr
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to present Vulkan swapchain image!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEnginePresentError;
		}

		return std::nullopt;
	}
}
