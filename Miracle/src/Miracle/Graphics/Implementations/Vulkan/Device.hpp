#pragma once

#include <variant>

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

		std::variant<MiracleError, vk::raii::ShaderModule> createShaderModule(
			const vk::ShaderModuleCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::PipelineLayout> createPipelineLayout(
			const vk::PipelineLayoutCreateInfo& createInfo
		) const;

		std::variant<MiracleError, vk::raii::Pipeline> createGraphicsPipeline(
			const vk::GraphicsPipelineCreateInfo& createInfo
		) const;

		inline const DeviceSupportDetails& getSupportDetails() const { return m_supportDetails; }
	};
}
