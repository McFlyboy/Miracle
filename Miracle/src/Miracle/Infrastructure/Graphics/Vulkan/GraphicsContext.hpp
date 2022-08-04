#pragma once

#include <string_view>
#include <span>

#include <Miracle/Definitions.hpp>
#include <Miracle/Application/Graphics/IGraphicsContext.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsContext : public Application::IGraphicsContext {
	private:
		Application::ILogger& m_logger;

		vk::raii::Context m_context;
		vk::raii::Instance m_instance;
#ifdef MIRACLE_CONFIG_DEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger;
#endif

	public:
		GraphicsContext(
			const std::string_view& appName,
			Application::ILogger& logger
		);

		~GraphicsContext();

	private:
		vk::raii::Instance createInstance(const std::string_view& appName);

		void checkExtensionsAvailable(
			const std::span<const char*>& extensionNames
		) const;

#ifdef MIRACLE_CONFIG_DEBUG
		void checkValidationLayersAvailable(
			const std::span<const char*>& validationLayerNames
		) const;

		vk::raii::DebugUtilsMessengerEXT createDebugMessenger();

		vk::DebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo();

		static VKAPI_ATTR VkBool32 VKAPI_CALL logDebugMessage(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData
		);
#endif
	};
}
