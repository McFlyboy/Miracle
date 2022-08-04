#include "GraphicsContext.hpp"

#include <cstring>
#include <exception>
#include <array>
#include <vector>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsContext::GraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger
	) :
		m_logger(logger),
		m_instance(createInstance(appName))
#ifdef MIRACLE_CONFIG_DEBUG
		, m_debugMessenger(createDebugMessenger())
#endif
	{
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

#ifdef MIRACLE_CONFIG_DEBUG
		auto validationLayerNames = std::array{ "VK_LAYER_KHRONOS_validation" };
		checkValidationLayersAvailable(validationLayerNames);

		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

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
					.enabledLayerCount       = static_cast<uint32_t>(validationLayerNames.size()),
					.ppEnabledLayerNames     = validationLayerNames.data(),
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
}