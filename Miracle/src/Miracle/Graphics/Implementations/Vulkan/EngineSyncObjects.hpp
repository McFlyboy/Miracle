#pragma once

#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	struct EngineSyncObjects {
	private:
		const Device& m_device;

	public:
		vk::raii::Semaphore imageAvailable;
		vk::raii::Semaphore renderingFinished;
		vk::raii::Fence frameInFlight;

		EngineSyncObjects(const Device& device);

		std::optional<MiracleError> waitForFences(bool waitForAll) const {
			return m_device.waitForFences(*frameInFlight, waitForAll, UINT64_MAX);
		}

		std::optional<MiracleError> resetAllFences() const {
			return m_device.resetFences(*frameInFlight);
		}
	};
}
