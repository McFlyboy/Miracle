#pragma once

#include "Vulkan.hpp"
#include "DeviceInfo.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class DeviceExplorer {
	public:
		DeviceExplorer() = delete;

		static DeviceInfo getDeviceInfo(
			const vk::raii::PhysicalDevice& device,
			const vk::raii::SurfaceKHR& surface
		);

		static bool isDeviceSupported(const DeviceInfo& deviceInfo);

	private:
		static QueueFamilyIndices queryQueueFamilyIndices(
			const vk::raii::PhysicalDevice& device,
			const vk::raii::SurfaceKHR& surface
		);
	};
}
