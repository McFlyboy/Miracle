#pragma once

#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	struct FrameInFlight {
	private:
		const Device& m_device;

	public:
		vk::raii::Semaphore imageAvailable;
		vk::raii::Semaphore renderingFinished;
		vk::raii::Fence flightEnded;

		FrameInFlight(const Device& device);

		std::optional<MiracleError> waitForEndOfFlight() const {
			return m_device.waitForFences(*flightEnded, true, UINT64_MAX);
		}

		std::optional<MiracleError> beginFlight() const {
			return m_device.resetFences(*flightEnded);
		}
	};
}
