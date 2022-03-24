#include "Device.hpp"

#include <set>
#include <utility>
#include <array>

#include "Instance.hpp"
#include "PhysicalDeviceSelector.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	Device::Device(
		const Instance& instance,
		const Surface& surface
	) {
		auto physicalDevicesResult = instance.enumeratePhysicalDevices();

		if (physicalDevicesResult.index() == 0) {
			throw std::get<MiracleError>(physicalDevicesResult);
		}

		auto& physicalDevices = std::get<std::vector<vk::raii::PhysicalDevice>>(physicalDevicesResult);

		auto selectedPhysicalDeviceResult = PhysicalDeviceSelector::selectPhysicalDevice(
			physicalDevices,
			surface.getRawSurface()
		);

		if (selectedPhysicalDeviceResult.index() == 0) {
			throw std::get<MiracleError>(selectedPhysicalDeviceResult);
		}

		auto& selectedPhysicalDevice
			= std::get<std::reference_wrapper<vk::raii::PhysicalDevice>>(selectedPhysicalDeviceResult).get();

		m_supportDetails = PhysicalDeviceSelector::queryDeviceSupport(
			selectedPhysicalDevice,
			surface.getRawSurface()
		);

		auto uniqueQueueFamilyIndices = std::set{
			m_supportDetails.queueFamilyIndices.graphicsFamilyIndex.value(),
			m_supportDetails.queueFamilyIndices.presentFamilyIndex.value()
		};

		auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>();
		queueCreateInfos.reserve(uniqueQueueFamilyIndices.size());

		float queuePriority = 1.0f;

		for (auto& uniqueQueueFamilyIndex : uniqueQueueFamilyIndices) {
			queueCreateInfos.emplace_back(
				vk::DeviceQueueCreateInfo{
					.flags            = {},
					.queueFamilyIndex = uniqueQueueFamilyIndex,
					.queueCount       = 1,
					.pQueuePriorities = &queuePriority,
				}
			);
		}

		auto validationLayers = Instance::getValidationLayers();

		auto extensions = std::array{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		try {
			m_device = selectedPhysicalDevice.createDevice({
				.flags                   = {},
				.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size()),
				.pQueueCreateInfos       = queueCreateInfos.data(),
				.enabledLayerCount       = static_cast<uint32_t>(validationLayers.size()),
				.ppEnabledLayerNames     = validationLayers.data(),
				.enabledExtensionCount   = static_cast<uint32_t>(extensions.size()),
				.ppEnabledExtensionNames = extensions.data(),
				.pEnabledFeatures        = nullptr,
			});
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan device!");
			Logger::error(e.what());
			throw MiracleError::VulkanGraphicsEngineDeviceCreationError;
		}

		m_graphicsQueue = GraphicsQueue(
			m_device.getQueue(
				m_supportDetails.queueFamilyIndices.graphicsFamilyIndex.value(),
				0
			)
		);

		m_presentQueue = PresentQueue(
			m_device.getQueue(
				m_supportDetails.queueFamilyIndices.presentFamilyIndex.value(),
				0
			)
		);
	}

	std::variant<MiracleError, vk::raii::SwapchainKHR> Device::createSwapchain(
		const vk::SwapchainCreateInfoKHR& createInfo
	) const {
		try {
			return m_device.createSwapchainKHR(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan swapchain!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineSwapchainCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::ImageView> Device::createImageView(
		const vk::ImageViewCreateInfo& createInfo
	) const {
		try {
			return m_device.createImageView(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan image view!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineImageViewCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::RenderPass> Device::createRenderPass(
		const vk::RenderPassCreateInfo& createInfo
	) const {
		try {
			return m_device.createRenderPass(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan render pass!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineRenderPassCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::ShaderModule> Device::createShaderModule(
		const vk::ShaderModuleCreateInfo& createInfo
	) const {
		try {
			return m_device.createShaderModule(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan shader module!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineShaderModuleCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::PipelineLayout> Device::createPipelineLayout(
		const vk::PipelineLayoutCreateInfo& createInfo
	) const {
		try {
			return m_device.createPipelineLayout(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan pipeline layout!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEnginePipelineLayoutCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::Pipeline> Device::createGraphicsPipeline(
		const vk::GraphicsPipelineCreateInfo& createInfo
	) const {
		try {
			return m_device.createGraphicsPipeline(nullptr, createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan graphics pipeline!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineGraphicsPipelineCreationError;
		}
	}
}
