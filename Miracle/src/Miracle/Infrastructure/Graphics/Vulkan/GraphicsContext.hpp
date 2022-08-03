#pragma once

#include <string_view>

#include <Miracle/Application/Graphics/IGraphicsContext.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsContext : public Application::IGraphicsContext {
	private:
		vk::raii::Context m_context;
		vk::raii::Instance m_instance;

	public:
		GraphicsContext(
			const std::string_view& appName,
			Application::ILogger& logger
		);

	private:
		vk::raii::Instance createInstance(
			const std::string_view& appName,
			Application::ILogger& logger
		) const;
	};
}
