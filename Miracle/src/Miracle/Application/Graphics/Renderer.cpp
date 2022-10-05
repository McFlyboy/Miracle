#include <Miracle/Application/Graphics/Renderer.hpp>

namespace Miracle::Application {
	Renderer::Renderer(
		const std::string_view& appName,
		ILogger& logger,
		IFileAccess& fileAccess,
		IGraphicsApi& api,
		IContextTarget& contextTarget,
		const RendererInitProps& initProps
	) :
		m_logger(logger),
		m_fileAccess(fileAccess),
		m_api(api),
		m_contextTarget(contextTarget),
		m_context(m_api.createGraphicsContext(appName, m_logger, m_contextTarget)),
		m_swapchain(m_api.createSwapchain(m_logger, *m_context.get(), initProps.swapchainInitProps)),
		m_pipeline(m_api.createGraphicsPipeline(m_logger, m_fileAccess, *m_context.get(), *m_swapchain.get())),
		m_vertexBuffer(m_api.createVertexBuffer(m_logger, *m_context.get()))
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

		auto swapchainImageSize = m_swapchain->getImageSize();

		m_context->recordCommands(
			[&]() {
				m_swapchain->beginRenderPass(0.125f, 0.125f, 0.125f);
				m_context->setViewport(0.0f, 0.0f, swapchainImageSize.width, swapchainImageSize.height);
				m_context->setScissor(0, 0, swapchainImageSize.width, swapchainImageSize.height);

				m_pipeline->bind();

				m_context->draw();

				m_swapchain->endRenderPass();
			}
		);

		m_context->submitRecording();

		m_swapchain->swap();
	}
}
