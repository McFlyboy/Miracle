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

		inline const DeviceSupportDetails& getSupportDetails() const { return m_supportDetails; }
	};
}