#include "PhysicalDeviceSelector.hpp"

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
			.queueFamilyIndices = queryQueueFamilyIndices(physicalDevice, supportedSurface)
		};
	}

	QueueFamilyIndices PhysicalDeviceSelector::queryQueueFamilyIndices(
		const vk::raii::PhysicalDevice& physicalDevice,
		const vk::raii::SurfaceKHR& supportedSurface
	) {
		auto queueFamilyIndices = QueueFamilyIndices();

		auto queueFamilyPropertiesList = physicalDevice.getQueueFamilyProperties();

		for (size_t i = 0; i < queueFamilyPropertiesList.size(); i++) {
			if (queueFamilyPropertiesList[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queueFamilyIndices.graphicsFamilyIndex = i;
			}

			try {
				auto isSurfaceSupported = physicalDevice.getSurfaceSupportKHR(i, *supportedSurface);

				if (isSurfaceSupported) {
					queueFamilyIndices.presentFamilyIndex = i;
				}
			}
			catch (const std::exception&) {}

			if (queueFamilyIndices.hasAll()) {
				break;
			}
		}

		return std::move(queueFamilyIndices);
	}
}
