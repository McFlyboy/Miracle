#include "EngineSyncObjects.hpp"

#include <utility>

namespace Miracle::Graphics::Implementations::Vulkan {
	EngineSyncObjects::EngineSyncObjects(const Device& device) :
		m_device(device),
		imageAvailable(nullptr),
		renderingFinished(nullptr),
		frameInFlight(nullptr)
	{
		auto imageAvailableCreateResult = device.createSemaphore({
			.flags = {}
		});

		if (imageAvailableCreateResult.index() == 0) {
			throw std::get<MiracleError>(imageAvailableCreateResult);
		}

		imageAvailable = std::move(std::get<vk::raii::Semaphore>(imageAvailableCreateResult));

		auto renderingFinishedCreateResult = device.createSemaphore({
			.flags = {}
			});

		if (renderingFinishedCreateResult.index() == 0) {
			throw std::get<MiracleError>(renderingFinishedCreateResult);
		}

		renderingFinished = std::move(std::get<vk::raii::Semaphore>(renderingFinishedCreateResult));

		auto frameInFlightCreateResult = device.createFence({
			.flags = vk::FenceCreateFlagBits::eSignaled
		});

		if (frameInFlightCreateResult.index() == 0) {
			throw std::get<MiracleError>(frameInFlightCreateResult);
		}

		frameInFlight = std::move(std::get<vk::raii::Fence>(frameInFlightCreateResult));
	}
}
