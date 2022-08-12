#include "Renderer.hpp"

namespace Miracle::Application {
	Renderer::Renderer(
		const std::string_view& appName,
		ILogger& logger,
		IGraphicsApi& api,
		IContextTarget& contextTarget,
		const RendererInitProps& initProps
	) :
		m_logger(logger),
		m_api(api),
		m_context(m_api.createGraphicsContext(appName, m_logger, contextTarget)),
		m_swapchain(m_api.createSwapchain(m_logger, *m_context.get(), initProps.swapchainInitProps))
	{}
}
