#include <Miracle/Application/Graphics/Renderer.hpp>

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
		m_contextTarget(contextTarget),
		m_context(m_api.createGraphicsContext(appName, m_logger, m_contextTarget)),
		m_swapchain(m_api.createSwapchain(m_logger, *m_context.get(), initProps.swapchainInitProps))
	{
		m_logger.info("Renderer created");
	}

	Renderer::~Renderer() {
		m_logger.info("Destroying renderer...");

		m_context->waitForDeviceIdle();
	}

	void Renderer::render() {
		if (!m_contextTarget.isCurrentlyPresentable()) [[unlikely]] return;

		if (m_contextTarget.isSizeChanged()) [[unlikely]] {
			m_context->waitForDeviceIdle();
			m_swapchain->recreate();
		}

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
