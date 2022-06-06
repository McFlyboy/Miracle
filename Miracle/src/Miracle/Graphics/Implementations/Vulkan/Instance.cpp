#include "Instance.hpp"

#include <utility>

#include <Miracle/MiracleError.hpp>
#include <Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	Instance::Instance(const vk::raii::Context& context, ISurfaceTarget& surfaceTarget) :
		m_surfaceTarget(surfaceTarget)
	{
		auto appInfo = vk::ApplicationInfo{
			.pApplicationName   = "Miracle Application",
			.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
			.pEngineName        = "Miracle",
			.engineVersion      = VK_MAKE_VERSION(0, 0, 0),
			.apiVersion         = VK_API_VERSION_1_0
		};

		auto validationLayers = getValidationLayers();

		auto extensions = std::vector<const char*>{
#ifndef NDEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
		};

		auto surfaceTargetExtensions = m_surfaceTarget.getRequiredInstanceExtensions();
		extensions.reserve(surfaceTargetExtensions.size());

		for (auto& surfaceTargetExtension : surfaceTargetExtensions) {
			extensions.push_back(surfaceTargetExtension);
		}

#ifndef NDEBUG
		auto debugMessengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT{
			.flags           = {},
			.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			.messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
				| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
				| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			.pfnUserCallback = debugCallback,
			.pUserData       = nullptr,
		};
#endif

		try {
			m_instance = context.createInstance({
#ifndef NDEBUG
				.pNext                   = &debugMessengerCreateInfo,
#endif
				.flags                   = {},
				.pApplicationInfo        = &appInfo,
				.enabledLayerCount       = static_cast<uint32_t>(validationLayers.size()),
				.ppEnabledLayerNames     = validationLayers.data(),
				.enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
				.ppEnabledExtensionNames = extensions.data()
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan instance!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineInstanceCreationError;
		}

#ifndef NDEBUG
		try {
			m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(debugMessengerCreateInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan debug messenger!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineDebugMessengerCreationError;
		}
#endif
	}

	std::variant<MiracleError, vk::raii::SurfaceKHR> Instance::createSurface() const {
		return m_surfaceTarget.createSurface(m_instance);
	}

	std::variant<MiracleError, std::vector<vk::raii::PhysicalDevice>> Instance::enumeratePhysicalDevices() const {
		try {
			return m_instance.enumeratePhysicalDevices();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to enumerate Vulkan physical devices!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEnginePhysicalDeviceEnumerationError;
		};
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	) {
		switch (messageSeverity) {
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			Logger::error(callbackData->pMessage);
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			Logger::warning(callbackData->pMessage);
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			Logger::info(callbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}
}
