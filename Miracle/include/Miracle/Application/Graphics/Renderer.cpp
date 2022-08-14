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

	Renderer::~Renderer() {
		m_context->waitForDeviceIdle();
	}

	void Renderer::render() {
		m_context->recordCommands(
			[&]() {
				m_swapchain->beginRenderPassCommand(0.125f, 0.125f, 0.125f);
				m_swapchain->endRenderPassCommand();
			}
		);

		m_context->submitRecording();

		m_swapchain->swap();
	}
}
