#pragma once

#include <variant>
#include <optional>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "DeviceSupportDetails.hpp"
#include "GraphicsQueue.hpp"
#include "PresentQueue.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Device {
	private:
		const Surface& m_surface;

		vk::raii::PhysicalDevice m_physicalDevice = nullptr;
		DeviceSupportDetails m_supportDetails;
		vk::raii::Device m_device = nullptr;
		GraphicsQueue m_graphicsQueue;
		PresentQueue m_presentQueue;

	public:
		Device(
			const Instance& instance,
			const Surface& surface
		);

		std::variant<MiracleError, vk::raii::SwapchainKHR> createSwapchain(
			const vk::SwapchainCreateInfoKHR& createInfo
		) const;

		std::variant<MiracleError, vk::raii::ImageView> createImageView(
			const vk::ImageViewCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::RenderPass> createRenderPass(
			const vk::RenderPassCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::Framebuffer> createFramebuffer(
			const vk::FramebufferCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::ShaderModule> createShaderModule(
			const vk::ShaderModuleCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::PipelineLayout> createPipelineLayout(
			const vk::PipelineLayoutCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::Pipeline> createGraphicsPipeline(
			const vk::GraphicsPipelineCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::Semaphore> createSemaphore(
			const vk::SemaphoreCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::Fence> createFence(
			const vk::FenceCreateInfo& createInfo
		) const;

		std::optional<MiracleError> waitForFences(
			const vk::ArrayProxy<const vk::Fence>& fences,
			bool waitForAll,
			uint64_t timeout
		) const;

		std::optional<MiracleError> resetFences(
			const vk::ArrayProxy<const vk::Fence>& fences
		) const;

		std::optional<MiracleError> waitIdle() const;

		void refreshSupportDetails();

		inline const DeviceSupportDetails& getSupportDetails() const { return m_supportDetails; }

		inline const GraphicsQueue& getGraphicsQueue() const { return m_graphicsQueue; }

		inline const PresentQueue& getPresentQueue() const { return m_presentQueue; }
	};
}
