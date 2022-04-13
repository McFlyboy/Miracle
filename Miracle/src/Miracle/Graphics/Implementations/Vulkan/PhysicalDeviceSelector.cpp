#include "PhysicalDeviceSelector.hpp"

#include <cstring>
#include <optional>
#include <utility>

#include <fmt/format.h>

#include <Miracle/MiracleError.hpp>
#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	std::variant<MiracleError, std::reference_wrapper<vk::raii::PhysicalDevice>> PhysicalDeviceSelector::selectPhysicalDevice(
		std::vector<vk::raii::PhysicalDevice>& physicalDevices,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		auto supportedPhysicalDevices = std::vector<std::reference_wrapper<vk::raii::PhysicalDevice>>();

		for (auto& physicalDevice : physicalDevices) {
			if (queryDeviceSupport(physicalDevice, supportedSurface).meetsAllRequirements()) {
				supportedPhysicalDevices.push_back(physicalDevice);
			}
		}

		if (supportedPhysicalDevices.empty()) {
			Logger::error("No Vulkan devices supported!");
			return MiracleError::VulkanGraphicsEngineNoPhysicalDevicesSupportedError;
		}

		std::optional<std::reference_wrapper<vk::raii::PhysicalDevice>> discreteGpu = std::nullopt;

		for (auto& supportedPhysicalDevice : supportedPhysicalDevices) {
			if (
				supportedPhysicalDevice.get().getProperties().deviceType ==
					vk::PhysicalDeviceType::eDiscreteGpu
			) {
				discreteGpu = supportedPhysicalDevice;
				break;
			}
		}

		auto selectedPhysicalDevice = discreteGpu.value_or(supportedPhysicalDevices.front());

		Logger::info(
			fmt::format(
				"Found Vulkan supported device: {}",
				selectedPhysicalDevice.get().getProperties().deviceName.data()
			)
		);
		return selectedPhysicalDevice;
	}

	DeviceSupportDetails PhysicalDeviceSelector::queryDeviceSupport(
		const vk::raii::PhysicalDevice& physicalDevice,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		return DeviceSupportDetails{
			.queueFamilyIndices      = queryQueueFamilyIndices(physicalDevice, supportedSurface),
			.extensionsSupported     = queryExtensionsSupported(physicalDevice),
			.memoryProperties        = physicalDevice.getMemoryProperties(),
			.swapchainSupportDetails = querySwapchainSupportDetails(physicalDevice, supportedSurface)
		};
	}

	QueueFamilyIndices PhysicalDeviceSelector::queryQueueFamilyIndices(
		const vk::raii::PhysicalDevice& physicalDevice,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		auto queueFamilyIndices = QueueFamilyIndices();

		auto queueFamilyPropertiesList = physicalDevice.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamilyPropertiesList.size(); i++) {
			auto& queueFamilyProperties = queueFamilyPropertiesList[i];

			if (
				!queueFamilyIndices.graphicsFamilyIndex.has_value() && (
					queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics
				)
			) {
				queueFamilyIndices.graphicsFamilyIndex = static_cast<uint32_t>(i);
			}

			if (!queueFamilyIndices.presentFamilyIndex.has_value()) {
				try {
					auto isSurfaceSupported = physicalDevice.getSurfaceSupportKHR(i, *supportedSurface);

					if (isSurfaceSupported) {
						queueFamilyIndices.presentFamilyIndex = static_cast<uint32_t>(i);
					}
				}
				catch (const std::exception&) {}
			}

			if (queueFamilyIndices.hasAll()) {
				break;
			}
		}

		return std::move(queueFamilyIndices);
	}

	ExtensionsSupported PhysicalDeviceSelector::queryExtensionsSupported(
		const vk::raii::PhysicalDevice& physicalDevice
	) {
		auto extensionsSupported = ExtensionsSupported();

		auto extensionPropertiesList = physicalDevice.enumerateDeviceExtensionProperties();

		for (auto& extensionProperties : extensionPropertiesList) {
			if (std::strcmp(extensionProperties.extensionName.data(), VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
				extensionsSupported.khrSwapchainExtensionSupported = true;
			}
		}

		return extensionsSupported;
	}

	SwapchainSupportDetails PhysicalDeviceSelector::querySwapchainSupportDetails(
		const vk::raii::PhysicalDevice& physicalDevice,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		return SwapchainSupportDetails{
			.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(*supportedSurface),
			.formats = physicalDevice.getSurfaceFormatsKHR(*supportedSurface),
			.presentModesSupported = queryPresentModesSupported(physicalDevice, supportedSurface)
		};
	}

	PresentModesSupported PhysicalDeviceSelector::queryPresentModesSupported(
		const vk::raii::PhysicalDevice& physicalDevice,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		auto presentModesSupported = PresentModesSupported();

		auto presentModes = physicalDevice.getSurfacePresentModesKHR(*supportedSurface);

		for (auto& presentMode : presentModes) {
			switch (presentMode) {
			case vk::PresentModeKHR::eImmediate:
				presentModesSupported.immediateModeSupported = true;
				break;

			case vk::PresentModeKHR::eFifo:
				presentModesSupported.fifoModeSupported = true;
				break;

			case vk::PresentModeKHR::eMailbox:
				presentModesSupported.mailboxModeSupported = true;
				break;
			}

			if (presentModesSupported.supportsAll()) {
				break;
			}
		}

		return presentModesSupported;
	}
}
