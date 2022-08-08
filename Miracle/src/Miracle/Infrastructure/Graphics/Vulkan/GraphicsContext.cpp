#include "GraphicsContext.hpp"

#include <cstring>
#include <exception>
#include <vector>
#include <algorithm>

#include <fmt/format.h>

#include "DeviceExplorer.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsContext::GraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger,
		IContextTarget& target
	) :
		m_logger(logger),
		m_target(target)
	{
		m_instance = createInstance(appName);
#ifdef MIRACLE_CONFIG_DEBUG
		m_debugMessenger = createDebugMessenger();
#endif
		m_surface = target.createVulkanSurface(m_instance);

		auto [physicalDevice, deviceInfo] = getMostOptimalPhysicalDevice();

		m_deviceInfo = std::move(deviceInfo);
		m_device = createDevice(physicalDevice);

		m_graphicsQueue = m_device.getQueue(
			m_deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
			0
		);

		m_presentQueue = m_device.getQueue(
			m_deviceInfo.queueFamilyIndices.presentFamilyIndex.value(),
			0
		);

		m_logger.info("Vulkan graphics context created");
	}

	GraphicsContext::~GraphicsContext() {
		m_logger.info("Destroying Vulkan graphics context...");
	}

	vk::raii::Instance GraphicsContext::createInstance(const std::string_view& appName) {
		auto appInfo = vk::ApplicationInfo{
			.pApplicationName   = appName.data(),
			.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
			.pEngineName        = "Miracle",
			.engineVersion      = VK_MAKE_VERSION(0, 0, 0),
			.apiVersion         = VK_API_VERSION_1_0
		};

		auto extensionNames = std::vector<const char*>();

		auto extensionNamesRequiredByTarget = m_target.getRequiredVulkanExtensionNames();

		for (auto& extensionName : extensionNamesRequiredByTarget) {
			extensionNames.push_back(extensionName);
		}

#ifdef MIRACLE_CONFIG_DEBUG
		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		checkValidationLayersAvailable(s_validationLayerNames);

		auto debugMessengerCreateInfo = getDebugMessengerCreateInfo();
#endif

		checkExtensionsAvailable(extensionNames);

		try {
			return m_context.createInstance(
				vk::InstanceCreateInfo{
#ifdef MIRACLE_CONFIG_DEBUG
					.pNext                   = &debugMessengerCreateInfo,
#endif
					.flags                   = {},
					.pApplicationInfo        = &appInfo,
#ifdef MIRACLE_CONFIG_DEBUG
					.enabledLayerCount       = static_cast<uint32_t>(s_validationLayerNames.size()),
					.ppEnabledLayerNames     = s_validationLayerNames.data(),
#endif
					.enabledExtensionCount   = static_cast<uint32_t>(extensionNames.size()),
					.ppEnabledExtensionNames = extensionNames.data()
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format(
					"Failed to create Vulkan instance.\n{}",
					e.what()
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	void GraphicsContext::checkExtensionsAvailable(
		const std::span<const char*>& extensionNames
	) const {
		bool allExtensionsFound = true;

		auto extensionsProperties = m_context.enumerateInstanceExtensionProperties();

		for (auto& extensionName : extensionNames) {
			bool extensionFound = false;

			for (auto& extensionProperties : extensionsProperties) {
				if (std::strcmp(extensionName, extensionProperties.extensionName) == 0) {
					extensionFound = true;
					break;
				}
			}

			if (extensionFound) {
				continue;
			}

			m_logger.error(fmt::format("Vulkan extension missing: {}", extensionName));
			allExtensionsFound = false;
		}

		if (allExtensionsFound) {
			return;
		}

		throw Application::GraphicsContextErrors::FunctionalityNotSupportedError();
	}

#ifdef MIRACLE_CONFIG_DEBUG
	void GraphicsContext::checkValidationLayersAvailable(
		const std::span<const char*>& validationLayerNames
	) const {
		bool allLayersFound = true;

		auto layersProperties = m_context.enumerateInstanceLayerProperties();

		for (auto& validationLayerName : validationLayerNames) {
			bool layerFound = false;

			for (auto& layerProperties : layersProperties) {
				if (std::strcmp(validationLayerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (layerFound) {
				continue;
			}

			m_logger.error(fmt::format("Vulkan validation layer missing: {}", validationLayerName));
			allLayersFound = false;
		}

		if (allLayersFound) {
			return;
		}

		throw Application::GraphicsContextErrors::DebugToolsUnavailableError();
	}

	vk::raii::DebugUtilsMessengerEXT GraphicsContext::createDebugMessenger() {
		try {
			return m_instance.createDebugUtilsMessengerEXT(getDebugMessengerCreateInfo());
		}
		catch (const std::exception& e) {
			m_logger.error(
				fmt::format(
					"Failed to create Vulkan debug messenger.\n{}",
					e.what()
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vk::DebugUtilsMessengerCreateInfoEXT GraphicsContext::getDebugMessengerCreateInfo() {
		return vk::DebugUtilsMessengerCreateInfoEXT{
			.flags = {},
			.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
				| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
				| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			.pfnUserCallback = logDebugMessage,
			.pUserData = this
		};
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsContext::logDebugMessage(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	) {
		switch (messageSeverity) {
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.error(callbackData->pMessage);
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.warning(callbackData->pMessage);
			break;

		default:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.info(callbackData->pMessage);
		}

		return false;
	}
#endif

	std::pair<vk::raii::PhysicalDevice, DeviceInfo> GraphicsContext::getMostOptimalPhysicalDevice() const {
		auto allDevices = m_instance.enumeratePhysicalDevices();
		
		if (allDevices.empty()) {
			m_logger.error("No Vulkan physical devices found");
			throw Application::GraphicsContextErrors::GraphicsDeviceNotFoundError();
		}

		m_logger.info(
			fmt::format("Vulkan physical devices found: {}", allDevices.size())
		);

		auto supportedDevices = std::vector<std::pair<vk::raii::PhysicalDevice*, DeviceInfo>>();
		supportedDevices.reserve(allDevices.size());

		for (auto& device : allDevices) {
			auto deviceInfo = DeviceExplorer::getDeviceInfo(device, m_surface);

			if (!DeviceExplorer::isDeviceSupported(deviceInfo)) continue;

			supportedDevices.emplace_back(&device, std::move(deviceInfo));
		}

		if (supportedDevices.empty()) {
			m_logger.error("No Vulkan physical devices supported");
			throw Application::GraphicsContextErrors::NoGraphicsDeviceSupportedError();
		}

		m_logger.info(
			fmt::format("Supported Vulkan physical devices: {}", supportedDevices.size())
		);

		std::sort(
			supportedDevices.begin(),
			supportedDevices.end(),
			[](
				std::pair<vk::raii::PhysicalDevice*, DeviceInfo>& lhs,
				std::pair<vk::raii::PhysicalDevice*, DeviceInfo>& rhs
			) {
				bool isLhsDiscreteGpu = lhs.second.type == vk::PhysicalDeviceType::eDiscreteGpu;
				bool isRhsDiscreteGpu = rhs.second.type == vk::PhysicalDeviceType::eDiscreteGpu;

				if (isLhsDiscreteGpu && !isRhsDiscreteGpu) {
					return true;
				}
				else if (isRhsDiscreteGpu && !isLhsDiscreteGpu) {
					return false;
				}
				else {
					return lhs.second.deviceLocalMemorySize > rhs.second.deviceLocalMemorySize;
				}
			}
		);

		auto& [selectedDevice, deviceInfo] = supportedDevices.front();

		m_logger.info(
			fmt::format(
				"Selected Vulkan device: {} [{}]",
				deviceInfo.name,
				vk::to_string(deviceInfo.type)
			)
		);

		return std::pair(std::move(*selectedDevice), std::move(deviceInfo));
	}

	vk::raii::Device GraphicsContext::createDevice(
		const vk::raii::PhysicalDevice& physicalDevice
	) const {
		float queuePriority = 1.0f;

		auto uniqueQueueFamilyIndices = m_deviceInfo.queueFamilyIndices.createSetOfAllUniqueIndices();

		auto deviceQueueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>();
		deviceQueueCreateInfos.reserve(uniqueQueueFamilyIndices.size());

		for (auto& queueFamilyIndex : uniqueQueueFamilyIndices) {
			deviceQueueCreateInfos.push_back(
				vk::DeviceQueueCreateInfo{
					.flags            = {},
					.queueFamilyIndex = queueFamilyIndex,
					.queueCount       = 1,
					.pQueuePriorities = &queuePriority
				}
			);
		}

		auto extensionNames = std::array{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		try {
			return physicalDevice.createDevice(
				vk::DeviceCreateInfo{
					.flags                   = {},
					.queueCreateInfoCount    = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
					.pQueueCreateInfos       = deviceQueueCreateInfos.data(),
#ifdef MIRACLE_CONFIG_DEBUG
					.enabledLayerCount = static_cast<uint32_t>(s_validationLayerNames.size()),
					.ppEnabledLayerNames = s_validationLayerNames.data(),
#endif
					.enabledExtensionCount   = static_cast<uint32_t>(extensionNames.size()),
					.ppEnabledExtensionNames = extensionNames.data(),
					.pEnabledFeatures        = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan device.\n{}", e.what()));
			throw Application::GraphicsContextErrors::CreationError();
		}
	}
}
