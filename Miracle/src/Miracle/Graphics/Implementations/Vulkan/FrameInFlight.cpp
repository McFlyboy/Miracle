#include "FrameInFlight.hpp"

#include <utility>

namespace Miracle::Graphics::Implementations::Vulkan {
	FrameInFlight::FrameInFlight(const Device& device) :
		m_device(device),
		imageAvailable(nullptr),
		renderingFinished(nullptr),
		flightEnded(nullptr)
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

		auto flightEndedCreateResult = device.createFence({
			.flags = vk::FenceCreateFlagBits::eSignaled
		});

		if (flightEndedCreateResult.index() == 0) {
			throw std::get<MiracleError>(flightEndedCreateResult);
		}

		flightEnded = std::move(std::get<vk::raii::Fence>(flightEndedCreateResult));
	}
}
