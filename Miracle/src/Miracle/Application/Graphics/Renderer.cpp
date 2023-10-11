#include <Miracle/Application/Graphics/Renderer.hpp>

#include <Miracle/Common/Components/Camera.hpp>

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
		m_meshBuffersList(createMeshBuffersList(initProps.meshes))
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

		const Transform* sceneCameraTransform = nullptr;
		const Camera* sceneCamera = nullptr;

		scene.forEachEntityCamera(
			[&](const Transform& transform, const Camera& camera) {
				sceneCameraTransform = &transform;
				sceneCamera = &camera;
			}
		);

		auto view = sceneCameraTransform != nullptr
			? Matrix4::createTranslation(-sceneCameraTransform->getTranslation())
				* Matrix4::createRotation(sceneCameraTransform->getRotation().getInverse())
			: Matrix4s::identity;

		auto projection = sceneCamera != nullptr
			? sceneCamera->getProjectionType() == CameraProjectionType::perspective
				? Matrix4::createPerspectiveProjection(
					aspectRatio,
					sceneCamera->getZoomFactor(),
					sceneCamera->getNearClipPlaneDistance(),
					sceneCamera->getFarClipPlaneDistance()
				)
				: Matrix4::createOrthographicProjection(
					aspectRatio,
					sceneCamera->getZoomFactor() * 0.2f,
					sceneCamera->getNearClipPlaneDistance(),
					sceneCamera->getFarClipPlaneDistance()
				)
			: Matrix4::createOrthographicProjection(
				aspectRatio,
				0.2f,
				0.0f,
				1000.0f
			);

		auto viewProjection = view * projection;

		m_context.recordCommands(
			[&]() {
				m_swapchain->beginRenderPass(scene.getBackgroundColor());
				m_context.setViewport(0.0f, 0.0f, swapchainImageSize.width, swapchainImageSize.height);
				m_context.setScissor(0, 0, swapchainImageSize.width, swapchainImageSize.height);

				if (!m_meshBuffersList.empty()) {
					scene.forEachEntityAppearance(
						[&](const Transform& transform, const Appearance& appearance) {
							if (!appearance.isVisible()) [[unlikely]] return;

							m_pipeline->bind();
							m_pipeline->pushConstants(
								PushConstants{
									.vertexStageConstants = VertexStagePushConstants{
										.transform = (transform.getTransformation() * viewProjection)
											.toTransposed()
									},
									.fragmentStageConstants = FragmentStagePushConstants{
										.color = appearance.getColor()
									}
								}
							);

							auto& meshBuffers = m_meshBuffersList[appearance.getMeshIndex()];

							meshBuffers.vertexBuffer->bind();
							meshBuffers.indexBuffer->bind();

							m_context.drawIndexed(meshBuffers.indexBuffer->getIndexCount());
						}
					);
				}

				m_swapchain->endRenderPass();
			}
		);

		m_context.submitRecording();

		m_swapchain->swap();

		return true;
	}

	std::vector<MeshBuffers> Renderer::createMeshBuffersList(const std::vector<Mesh>& meshes) const {
		auto list = std::vector<MeshBuffers>();
		list.reserve(meshes.size());

		for (auto& mesh : meshes) {
			list.emplace_back(
				m_api.createVertexBuffer(m_context, mesh.vertices),
				m_api.createIndexBuffer(m_context, mesh.faces)
			);
		}

		return list;
	}
}
