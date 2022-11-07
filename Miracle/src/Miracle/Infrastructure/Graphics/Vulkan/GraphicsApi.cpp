#include "GraphicsApi.hpp"

#include "GraphicsContext.hpp"
#include "Swapchain.hpp"
#include "GraphicsPipeline.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsApi::GraphicsApi(Application::ILogger& logger) :
		m_logger(logger)
	{}

	std::unique_ptr<Application::IGraphicsContext> GraphicsApi::createGraphicsContext(
		const std::string_view& appName,
		Application::IContextTarget& target
	) const {
		return std::make_unique<GraphicsContext>(
			appName,
			m_logger,
			*reinterpret_cast<IContextTarget*>(&target)
		);
	}

	std::unique_ptr<Application::ISwapchain> GraphicsApi::createSwapchain(
		Application::IGraphicsContext& context,
		const Application::SwapchainInitProps& initProps
	) const {
		return std::make_unique<Swapchain>(
			m_logger,
			*reinterpret_cast<GraphicsContext*>(&context),
			initProps
		);
	}

	std::unique_ptr<Application::IGraphicsPipeline> GraphicsApi::createGraphicsPipeline(
		Application::IFileAccess& fileAccess,
		Application::IGraphicsContext& context,
		Application::ISwapchain& swapchain
	) const {
		return std::make_unique<GraphicsPipeline>(
			m_logger,
			fileAccess,
			*reinterpret_cast<GraphicsContext*>(&context),
			*reinterpret_cast<Swapchain*>(&swapchain)
		);
	}

	std::unique_ptr<Application::IVertexBuffer> GraphicsApi::createVertexBuffer(
		Application::IGraphicsContext& context,
		const std::vector<Vertex>& vertices
	) const {
		return std::make_unique<VertexBuffer>(
			m_logger,
			*reinterpret_cast<GraphicsContext*>(&context),
			vertices
		);
	}

	std::unique_ptr<Application::IIndexBuffer> GraphicsApi::createIndexBuffer(
		Application::IGraphicsContext& context,
		const std::vector<Face>& faces
	) const {
		return std::make_unique<IndexBuffer>(
			m_logger,
			*reinterpret_cast<GraphicsContext*>(&context),
			faces
		);
	}
}
