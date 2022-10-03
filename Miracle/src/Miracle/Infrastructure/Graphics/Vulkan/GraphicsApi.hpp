#pragma once

#include <Miracle/Application/Graphics/IGraphicsApi.hpp>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsApi : public Application::IGraphicsApi {
		virtual std::unique_ptr<Application::IGraphicsContext> createGraphicsContext(
			const std::string_view& appName,
			Application::ILogger& logger,
			Application::IContextTarget& target
		) const override;

		virtual std::unique_ptr<Application::ISwapchain> createSwapchain(
			Application::ILogger& logger,
			Application::IGraphicsContext& context,
			const Application::SwapchainInitProps& initProps
		) const override;

		virtual std::unique_ptr<Application::IGraphicsPipeline> createGraphicsPipeline(
			Application::ILogger& logger,
			Application::IGraphicsContext& context
		) const override;
	};
}
