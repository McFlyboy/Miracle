#include "DeviceExplorer.hpp"

#include <cstring>
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
			.queueFamilyIndices    = queryQueueFamilyIndices(device, surface),
			.extensionSupport      = queryExtensionSupport(device, surface)
		};
	}

	bool DeviceExplorer::isDeviceSupported(const DeviceInfo& deviceInfo) {
		return deviceInfo.queueFamilyIndices.graphicsFamilyIndex.has_value()
			&& deviceInfo.queueFamilyIndices.presentFamilyIndex.has_value()
			&& deviceInfo.extensionSupport.swapchainSupport.has_value()
			&& !deviceInfo.extensionSupport.swapchainSupport.value().surfaceFormats.empty()
			&& deviceInfo.extensionSupport.swapchainSupport.value().hasImmediateModePresentationSupport;
	}

	QueueFamilyIndices DeviceExplorer::queryQueueFamilyIndices(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto queueFamilyIndices = QueueFamilyIndices{};

		auto queueFamiliesProperties = device.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamiliesProperties.size(); i++) {
			if (queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queueFamilyIndices.graphicsFamilyIndex = static_cast<uint32_t>(i);
			}

			if (device.getSurfaceSupportKHR(i, *surface)) {
				queueFamilyIndices.presentFamilyIndex = static_cast<uint32_t>(i);
			}

			if (queueFamilyIndices.hasAllIndicesSet()) break;
		}

		return queueFamilyIndices;
	}

	DeviceExtensionSupport DeviceExplorer::queryExtensionSupport(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto extensionSupport = DeviceExtensionSupport{};

		for (auto& extensionProperties : device.enumerateDeviceExtensionProperties()) {
			if (std::strcmp(extensionProperties.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
				extensionSupport.swapchainSupport = querySwapchainSupport(device, surface);
			}
		}

		return extensionSupport;
	}

	SwapchainSupport DeviceExplorer::querySwapchainSupport(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto surfaceCapabilities = device.getSurfaceCapabilitiesKHR(*surface);
		auto presentModes = device.getSurfacePresentModesKHR(*surface);

		bool hasImmediateMode = false;
		bool hasMailboxMode = false;

		for (auto& presentMode : presentModes) {
			if (presentMode == vk::PresentModeKHR::eImmediate) {
				hasImmediateMode = true;
			}
			else if (presentMode == vk::PresentModeKHR::eMailbox) {
				hasMailboxMode = true;
			}
		}

		return SwapchainSupport{
			.minImageCount                       = surfaceCapabilities.minImageCount,
			.maxImageCount                       = surfaceCapabilities.maxImageCount != 0
				? std::optional(surfaceCapabilities.maxImageCount)
				: std::nullopt,
			.surfaceFormats                      = device.getSurfaceFormatsKHR(*surface),
			.hasImmediateModePresentationSupport = hasImmediateMode,
			.hasMailboxModePresentationSupport   = hasMailboxMode
		};
	}
}
