#include "GraphicsEngine.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsEngine::GraphicsEngine(
		const ISurfaceTarget& surfaceTarget,
		const Io::ResourceLoader& resourceLoader
	) :
		m_instance(m_context, surfaceTarget),
		m_surface(m_instance),
		m_device(m_instance, m_surface),
		m_swapchain(m_device, m_surface),
		m_graphicsPipeline(m_device, m_swapchain, resourceLoader),
		m_syncObjects(m_device)
	{
		Logger::info("Vulkan graphics engine created");
	}

	GraphicsEngine::~GraphicsEngine() {
		Logger::info("Destroying Vulkan graphics engine...");
	}

	std::optional<MiracleError> GraphicsEngine::render() {
		auto fencesWaitResult = m_syncObjects.waitForFences(true);

		if (fencesWaitResult.has_value()) {
			return fencesWaitResult.value();
		}

		auto fencesResetResult = m_syncObjects.resetAllFences();

		if (fencesResetResult.has_value()) {
			return fencesResetResult.value();
		}

		auto imageAcquireResult = m_swapchain.acquireNextImage(m_syncObjects.imageAvailable);

		if (imageAcquireResult.index() == 0) {
			return std::get<MiracleError>(imageAcquireResult);
		}

		auto& imageIndex = std::get<uint32_t>(imageAcquireResult);

		auto recordError = recordDrawCommands(imageIndex);

		if (recordError.has_value()) {
			return recordError.value();
		}

		auto submitError = m_device.getGraphicsQueue()
			.submitRecorded(
				m_syncObjects.imageAvailable,
				m_syncObjects.renderingFinished,
				m_syncObjects.frameInFlight
			);

		if (submitError.has_value()) {
			return submitError.value();
		}

		auto presentError = m_device.getPresentQueue()
			.present(
				m_syncObjects.renderingFinished,
				m_swapchain.getRawSwapchain(),
				imageIndex
			);

		if (presentError.has_value()) {
			return presentError.value();
		}

		return std::nullopt;
	}

	std::optional<MiracleError> GraphicsEngine::waitForExecutionToFinish() {
		return m_device.waitIdle();
	}

	std::optional<MiracleError> GraphicsEngine::recordDrawCommands(uint32_t imageIndex) {
		return m_device.getGraphicsQueue()
			.recordCommands(
				[this, &imageIndex](const vk::raii::CommandBuffer& commandBuffer) {
					m_swapchain.executeRenderPass(
						commandBuffer,
						imageIndex,
						vk::ClearColorValue(std::array{ 0.0f, 0.0f, 0.0f, 1.0f }),
						[this, &commandBuffer]() {
							m_graphicsPipeline.bind(commandBuffer);

							commandBuffer.draw(3, 1, 0, 0);
						}
					);
				}
			);
	}
}
