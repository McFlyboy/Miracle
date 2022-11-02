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
			Application::IFileAccess& fileAccess,
			Application::IGraphicsContext& context,
			Application::ISwapchain& swapchain
		) const override;

		virtual std::unique_ptr<Application::IVertexBuffer> createVertexBuffer(
			Application::ILogger& logger,
			Application::IGraphicsContext& context,
			const std::vector<Vertex>& vertices
		) const override;

		virtual std::unique_ptr<Application::IIndexBuffer> createIndexBuffer(
			Application::ILogger& logger,
			Application::IGraphicsContext& context,
			const std::vector<Face>& faces
		) const override;
	};
}
