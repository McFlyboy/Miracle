#include "DeviceExplorer.hpp"

#include <span>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	DeviceInfo DeviceExplorer::getDeviceInfo(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto properties = device.getProperties();
		auto memoryProperties = device.getMemoryProperties();

		auto memoryTypes = std::span(memoryProperties.memoryTypes.data(), memoryProperties.memoryTypeCount);

		vk::DeviceSize memorySize = 0;

		for (auto& memoryType : memoryTypes) {
			if (memoryType.propertyFlags & vk::MemoryPropertyFlagBits::eDeviceLocal) {
				memorySize += memoryProperties.memoryHeaps[memoryType.heapIndex].size;
			}
		}
		
		return DeviceInfo{
			.name                  = properties.deviceName,
			.type                  = properties.deviceType,
			.deviceLocalMemorySize = memorySize,
			.queueFamilyIndices    = queryQueueFamilyIndices(device, surface)
		};
	}

	bool DeviceExplorer::isDeviceSupported(const DeviceInfo& deviceInfo) {
		return deviceInfo.queueFamilyIndices.graphicsFamilyIndex.has_value()
			&& deviceInfo.queueFamilyIndices.presentFamilyIndex.has_value();
	}

	QueueFamilyIndices DeviceExplorer::queryQueueFamilyIndices(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto queueFamilyIndices = QueueFamilyIndices{};

		auto queueFamiliesProperties = device.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamiliesProperties.size(); i++) {
			if (queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queueFamilyIndices.graphicsFamilyIndex = i;
			}

			if (device.getSurfaceSupportKHR(i, *surface)) {
				queueFamilyIndices.presentFamilyIndex = i;
			}

			if (queueFamilyIndices.hasAllIndicesSet()) break;
		}

		return queueFamilyIndices;
	}
}
