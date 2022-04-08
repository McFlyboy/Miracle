#include "GraphicsEngine.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsEngine::GraphicsEngine(
		ISurfaceTarget& surfaceTarget,
		const Io::ResourceLoader& resourceLoader
	) :
		m_instance(m_context, surfaceTarget),
		m_surface(m_instance),
		m_device(m_instance, m_surface),
		m_swapchain(m_device, m_surface),
		m_graphicsPipeline(m_device, m_swapchain, resourceLoader),
		m_framesInFlight(m_device)
	{
		Logger::info("Vulkan graphics engine created");
	}

	GraphicsEngine::~GraphicsEngine() {
		Logger::info("Destroying Vulkan graphics engine...");
	}

	std::optional<MiracleError> GraphicsEngine::render() {
		if (m_surface.getSurfaceTarget().isExtentChanged()) {
			m_swapchain.setOutdated(true);
		}

		if (m_swapchain.isOutdated()) {
			if (m_swapchain.getImageExtent() != m_surface.getSurfaceTarget().getCurrentExtent()) {
				auto error = recreateSwapchainAndDependents();

				if (
					error.has_value()
					&& error.value() == MiracleError::VulkanGraphicsEngineSurfaceAreaEqualsZeroError
				) {
					return std::nullopt;
				}
			}
			else {
				m_swapchain.setOutdated(false);
			}
		}

		auto& targetFrameInFlight = m_framesInFlight.getTargetFrameInFlight();

		auto endOfFlightWaitResult = targetFrameInFlight.waitForEndOfFlight();

		if (endOfFlightWaitResult.has_value()) {
			return endOfFlightWaitResult.value();
		}

		auto imageAcquireResult = m_swapchain.acquireNextImage(targetFrameInFlight.imageAvailable);

		if (imageAcquireResult.index() == 0) {
			auto& error = std::get<MiracleError>(imageAcquireResult);

			if (error == MiracleError::VulkanGraphicsEngineSwapchainOutOfDateError) {
				m_swapchain.setOutdated(true);
			}

			return error;
		}

		auto& imageIndex = std::get<uint32_t>(imageAcquireResult);

		auto beginFlightResult = targetFrameInFlight.beginFlight();

		if (beginFlightResult.has_value()) {
			return beginFlightResult.value();
		}

		auto recordError = recordDrawCommands(m_framesInFlight.getTargetFrameInFlightIndex(), imageIndex);

		if (recordError.has_value()) {
			return recordError.value();
		}

		auto submitError = m_device.getGraphicsQueue()
			.submitRecorded(
				m_framesInFlight.getTargetFrameInFlightIndex(),
				targetFrameInFlight.imageAvailable,
				targetFrameInFlight.renderingFinished,
				targetFrameInFlight.flightEnded
			);

		if (submitError.has_value()) {
			return submitError.value();
		}

		auto presentError = m_device.getPresentQueue()
			.present(
				targetFrameInFlight.renderingFinished,
				m_swapchain.getRawSwapchain(),
				imageIndex
			);

		if (presentError.has_value()) {
			if (presentError.value() == MiracleError::VulkanGraphicsEngineSwapchainOutOfDateError) {
				m_swapchain.setOutdated(true);
			}

			return presentError.value();
		}

		m_framesInFlight.nextTarget();

		return std::nullopt;
	}

	std::optional<MiracleError> GraphicsEngine::waitForExecutionToFinish() {
		return m_device.waitIdle();
	}

	std::optional<MiracleError> GraphicsEngine::recordDrawCommands(
		int bufferIndex,
		uint32_t imageIndex
	) {
		return m_device.getGraphicsQueue()
			.recordCommands(
				bufferIndex,
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

	std::optional<MiracleError> GraphicsEngine::recreateSwapchainAndDependents() {
		auto currentExtent = m_surface.getSurfaceTarget().getCurrentExtent();

		if (currentExtent.width * currentExtent.height == 0) {
			return MiracleError::VulkanGraphicsEngineSurfaceAreaEqualsZeroError;
		}

		m_device.waitIdle();
		m_device.refreshSupportDetails();

		auto swapchainRecreateError = m_swapchain.recreate();

		if (swapchainRecreateError.has_value()) {
			Logger::error("Failed to rebuild Vulkan swapchain!");
			return swapchainRecreateError.value();
		}

		auto graphicsPipelineRecreateError = m_graphicsPipeline.recreate();

		if (graphicsPipelineRecreateError.has_value()) {
			Logger::error("Failed to rebuild Vulkan graphics pipeline!");
			return graphicsPipelineRecreateError.value();
		}

		Logger::info("Vulkan swapchain and dependents rebuilt");
		return std::nullopt;
	}
}
