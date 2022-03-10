#pragma once

#include <variant>
#include <functional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "DeviceSupportDetails.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class PhysicalDeviceSelector {
	public:
		static std::variant<MiracleError, std::reference_wrapper<vk::raii::PhysicalDevice>> selectPhysicalDevice(
			std::vector<vk::raii::PhysicalDevice>& physicalDevices,
			const vk::raii::SurfaceKHR& supportedSurface
		);

		static DeviceSupportDetails queryDeviceSupport(
			const vk::raii::PhysicalDevice& physicalDevice,
			const vk::raii::SurfaceKHR& supportedSurface
		);

	private:
		static QueueFamilyIndices queryQueueFamilyIndices(
			const vk::raii::PhysicalDevice& physicalDevice,
			const vk::raii::SurfaceKHR& supportedSurface
		);

		static ExtensionsSupported queryExtensionsSupported(
			const vk::raii::PhysicalDevice& physicalDevice
		);
	};
}
