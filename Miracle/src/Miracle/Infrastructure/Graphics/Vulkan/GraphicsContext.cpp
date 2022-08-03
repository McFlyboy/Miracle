#include "GraphicsContext.hpp"

#include <exception>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsContext::GraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger
	) :
		m_instance(createInstance(appName, logger))
	{
		logger.info("Vulkan graphics context created");
	}

	vk::raii::Instance GraphicsContext::createInstance(
		const std::string_view& appName,
		Application::ILogger& logger
	) const {
		auto appInfo = vk::ApplicationInfo{
			.pApplicationName   = appName.data(),
			.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
			.pEngineName        = "Miracle",
			.engineVersion      = VK_MAKE_VERSION(0, 0, 0),
			.apiVersion         = VK_API_VERSION_1_0
		};

		try {
			return m_context.createInstance(
				vk::InstanceCreateInfo{
					.flags                   = {},
					.pApplicationInfo        = &appInfo,
					.enabledLayerCount       = 0,
					.ppEnabledLayerNames     = nullptr,
					.enabledExtensionCount   = 0,
					.ppEnabledExtensionNames = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			logger.error(
				fmt::format(
					"Failed to create Vulkan instance.\n{}",
					e.what()
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}
}
