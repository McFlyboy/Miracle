#include <Miracle/Application/Graphics/Renderer.hpp>

namespace Miracle::Application {
	Renderer::Renderer(
		ILogger& logger,
		IFileAccess& fileAccess,
		IGraphicsApi& api,
		IGraphicsContext& context,
		const RendererInitProps& initProps
	) :
		m_logger(logger),
		m_fileAccess(fileAccess),
		m_api(api),
		m_context(context),
		m_swapchain(m_api.createSwapchain(m_context, initProps.swapchainInitProps)),
		m_pipeline(m_api.createGraphicsPipeline(m_fileAccess, m_context, *m_swapchain.get())),
		m_vertexBuffer(
			!initProps.mesh.vertices.empty()
				? m_api.createVertexBuffer(m_context, initProps.mesh.vertices)
				: nullptr
		),
		m_indexBuffer(
			!initProps.mesh.faces.empty()
				? m_api.createIndexBuffer(m_context, initProps.mesh.faces)
				: nullptr
		)
	{
		auto swapchainImageSize = m_swapchain->getImageSize();

		m_logger.info("Renderer created");
	}

	Renderer::~Renderer() {
		m_logger.info("Destroying renderer...");

		m_context.waitForDeviceIdle();
	}

	bool Renderer::render(const Scene& scene) {
		if (!m_context.getTarget().isCurrentlyPresentable()) [[unlikely]] return false;

		if (m_context.getTarget().isSizeChanged()) [[unlikely]] {
			m_context.waitForDeviceIdle();
			m_swapchain->recreate();
		}

		auto swapchainImageSize = m_swapchain->getImageSize();
		auto aspectRatio = static_cast<float>(swapchainImageSize.width)
			/ static_cast<float>(swapchainImageSize.height);

		m_context.recordCommands({
			[&]() {
				m_swapchain->beginRenderPass(scene.getBackgroundColor());
				m_context.setViewport(0.0f, 0.0f, swapchainImageSize.width, swapchainImageSize.height);
				m_context.setScissor(0, 0, swapchainImageSize.width, swapchainImageSize.height);

				scene.forEachEntityPosition(
					[&](const Vector2f& position) {
						m_pipeline->bind();
						m_pipeline->pushConstants(
							PushConstants{
								.translation = position,
								.aspectRatio = aspectRatio
							}
						);

						if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr) {
							m_vertexBuffer->bind();
							m_indexBuffer->bind();

							m_context.drawIndexed(m_indexBuffer->getIndexCount());
						}
					}
				);

				m_swapchain->endRenderPass();
			}
		});

		m_context.submitRecording();

		m_swapchain->swap();

		return true;
	}
}
