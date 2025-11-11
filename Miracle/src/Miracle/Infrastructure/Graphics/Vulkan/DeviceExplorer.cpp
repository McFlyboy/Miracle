#include "DeviceExplorer.hpp"

#include <cstring>
#include <span>
#include <vector>
#include <utility>

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

		auto depthStencilImageFormats = std::vector{
			vk::Format::eD16Unorm,
			vk::Format::eD16UnormS8Uint,
			vk::Format::eD24UnormS8Uint,
			vk::Format::eD32Sfloat,
			vk::Format::eD32SfloatS8Uint
		};

		auto supportedDepthStencilImageFormatsWithOptimalTiling = std::vector<vk::Format>();

		for (auto& depthStencilImageFormat : depthStencilImageFormats) {
			auto formatProperties = device.getFormatProperties(depthStencilImageFormat);

			if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
				supportedDepthStencilImageFormatsWithOptimalTiling.push_back(depthStencilImageFormat);
		}
		
		return DeviceInfo{
			.name                                           = properties.deviceName,
			.type                                           = properties.deviceType,
			.deviceLocalMemorySize                          = memorySize,
			.depthStencilOptimalTilingImageFormatsSupported = std::move(supportedDepthStencilImageFormatsWithOptimalTiling),
			.queueFamilyIndices                             = queryQueueFamilyIndices(device, surface),
			.extensionSupport                               = queryExtensionSupport(device, surface)
		};
	}

	bool DeviceExplorer::isDeviceSupported(const DeviceInfo& deviceInfo) {
		return !deviceInfo.depthStencilOptimalTilingImageFormatsSupported.empty()
			&& deviceInfo.queueFamilyIndices.graphicsFamilyIndex.has_value()
			&& deviceInfo.queueFamilyIndices.presentFamilyIndex.has_value()
			&& deviceInfo.queueFamilyIndices.transferFamilyIndex.has_value()
			&& deviceInfo.extensionSupport.swapchainSupport.has_value()
			&& deviceInfo.extensionSupport.swapchainSupport.value().hasDoubleBufferingSupport
			&& !deviceInfo.extensionSupport.swapchainSupport.value().surfaceFormats.empty()
			&& deviceInfo.extensionSupport.swapchainSupport.value().hasImmediateModePresentationSupport
			&& deviceInfo.extensionSupport.hasExtendedDynamicStateSupport;
	}

	QueueFamilyIndices DeviceExplorer::queryQueueFamilyIndices(
		const vk::raii::PhysicalDevice& device,
		const vk::raii::SurfaceKHR& surface
	) {
		auto queueFamilyIndices = QueueFamilyIndices{};

		auto queueFamilyPropertiesList = device.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamilyPropertiesList.size(); i++) {
			if (
				!queueFamilyIndices.graphicsFamilyIndex.has_value()
					&& queueFamilyPropertiesList[i].queueFlags & vk::QueueFlagBits::eGraphics
			) {
				queueFamilyIndices.graphicsFamilyIndex = static_cast<uint32_t>(i);
			}

			if (
				!queueFamilyIndices.presentFamilyIndex.has_value()
					&& device.getSurfaceSupportKHR(i, *surface)
			) {
				queueFamilyIndices.presentFamilyIndex = static_cast<uint32_t>(i);
			}

			if (
				!queueFamilyIndices.transferFamilyIndex.has_value()
					&& queueFamilyPropertiesList[i].queueFlags & vk::QueueFlagBits::eTransfer
					&& !(queueFamilyPropertiesList[i].queueFlags & vk::QueueFlagBits::eGraphics)
					&& !(queueFamilyPropertiesList[i].queueFlags & vk::QueueFlagBits::eCompute)
			) {
				queueFamilyIndices.transferFamilyIndex = static_cast<uint32_t>(i);
			}

			if (queueFamilyIndices.hasAllIndicesSet()) break;
		}

		if (!queueFamilyIndices.transferFamilyIndex.has_value()) {
			queueFamilyIndices.transferFamilyIndex = queueFamilyIndices.graphicsFamilyIndex;
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
			else if (std::strcmp(extensionProperties.extensionName, VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME) == 0) {
				extensionSupport.hasExtendedDynamicStateSupport = true;
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
			.hasDoubleBufferingSupport           = surfaceCapabilities.minImageCount <= 2
				&& (surfaceCapabilities.maxImageCount == 0 || surfaceCapabilities.maxImageCount >= 2),
			.hasTripleBufferingSupport           = surfaceCapabilities.minImageCount <= 3
				&& (surfaceCapabilities.maxImageCount == 0 || surfaceCapabilities.maxImageCount >= 3),
			.surfaceFormats                      = device.getSurfaceFormatsKHR(*surface),
			.hasImmediateModePresentationSupport = hasImmediateMode,
			.hasMailboxModePresentationSupport   = hasMailboxMode
		};
	}
}
