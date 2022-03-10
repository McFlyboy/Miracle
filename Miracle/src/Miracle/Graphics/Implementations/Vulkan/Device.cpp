#include "Device.hpp"

#include <set>
#include <utility>

#include "Instance.hpp"
#include "PhysicalDeviceSelector.hpp"

#include <Miracle/MiracleError.hpp>
#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	Device::Device(
		const Instance& instance,
		const Surface& surface
	) {
		auto physicalDevicesResult = instance.enumeratePhysicalDevices();

		if (physicalDevicesResult.index() == 0) {
			throw std::get<MiracleError>(physicalDevicesResult);
		}

		auto& physicalDevices = std::get<std::vector<vk::raii::PhysicalDevice>>(physicalDevicesResult);

		auto selectedPhysicalDeviceResult = PhysicalDeviceSelector::selectPhysicalDevice(
			physicalDevices,
			surface.getSurface()
		);

		if (selectedPhysicalDeviceResult.index() == 0) {
			throw std::get<MiracleError>(selectedPhysicalDeviceResult);
		}

		auto& selectedPhysicalDevice
			= std::get<std::reference_wrapper<vk::raii::PhysicalDevice>>(selectedPhysicalDeviceResult).get();

		m_supportDetails = std::move(PhysicalDeviceSelector::queryDeviceSupport(
			selectedPhysicalDevice,
			surface.getSurface()
		));

		auto uniqueQueueFamilyIndices = std::set{
			m_supportDetails.queueFamilyIndices.graphicsFamilyIndex.value(),
			m_supportDetails.queueFamilyIndices.presentFamilyIndex.value()
		};

		auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>();
		queueCreateInfos.reserve(uniqueQueueFamilyIndices.size());

		float queuePriority = 1.0f;

		for (auto& uniqueQueueFamilyIndex : uniqueQueueFamilyIndices) {
			queueCreateInfos.emplace_back(
				vk::DeviceQueueCreateInfo{
					.flags            = {},
					.queueFamilyIndex = uniqueQueueFamilyIndex,
					.queueCount       = 1,
					.pQueuePriorities = &queuePriority,
				}
			);
		}

		auto validationLayers = Instance::getValidationLayers();

		try {
			m_device = selectedPhysicalDevice.createDevice({
				.flags = {},
				.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size()),
				.pQueueCreateInfos       = queueCreateInfos.data(),
				.enabledLayerCount       = static_cast<uint32_t>(validationLayers.size()),
				.ppEnabledLayerNames     = validationLayers.data(),
				.enabledExtensionCount   = 0,
				.ppEnabledExtensionNames = nullptr,
				.pEnabledFeatures        = nullptr,
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan device!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineDeviceCreationError;
		}

		m_graphicsQueue = m_device.getQueue(
			m_supportDetails.queueFamilyIndices.graphicsFamilyIndex.value(),
			0
		);

		m_presentQueue = m_device.getQueue(
			m_supportDetails.queueFamilyIndices.presentFamilyIndex.value(),
			0
		);
	}
}
