#include "GraphicsApi.hpp"

#include "GraphicsContext.hpp"
#include "Swapchain.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	std::unique_ptr<Application::IGraphicsContext> GraphicsApi::createGraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger,
		Application::IContextTarget& target
	) const {
		return std::make_unique<GraphicsContext>(
			appName,
			logger,
			*reinterpret_cast<IContextTarget*>(&target)
		);
	}

	std::unique_ptr<Application::ISwapchain> GraphicsApi::createSwapchain(
		Application::ILogger& logger,
		Application::IGraphicsContext& context,
		const Application::SwapchainInitProps& initProps
	) const {
		return std::make_unique<Swapchain>(
			logger,
			*reinterpret_cast<GraphicsContext*>(&context),
			initProps
		);
	}
}
