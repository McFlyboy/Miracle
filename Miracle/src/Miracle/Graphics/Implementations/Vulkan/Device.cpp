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
	) :
		m_surface(surface)
	{
		auto physicalDevicesResult = instance.enumeratePhysicalDevices();

		if (physicalDevicesResult.index() == 0) {
			throw std::get<MiracleError>(physicalDevicesResult);
		}

		auto& physicalDevices = std::get<std::vector<vk::raii::PhysicalDevice>>(physicalDevicesResult);

		auto selectedPhysicalDeviceResult = PhysicalDeviceSelector::selectPhysicalDevice(
			physicalDevices,
			m_surface.getRawSurface()
		);

		if (selectedPhysicalDeviceResult.index() == 0) {
			throw std::get<MiracleError>(selectedPhysicalDeviceResult);
		}

		m_physicalDevice = std::move(
			std::get<std::reference_wrapper<vk::raii::PhysicalDevice>>(selectedPhysicalDeviceResult).get()
		);

		m_supportDetails = PhysicalDeviceSelector::queryDeviceSupport(
			m_physicalDevice,
			m_surface.getRawSurface()
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
			m_device = m_physicalDevice.createDevice({
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
			m_device,
			m_supportDetails.queueFamilyIndices.graphicsFamilyIndex.value()
		);

		m_presentQueue = PresentQueue(
			m_device,
			m_supportDetails.queueFamilyIndices.presentFamilyIndex.value()
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

	std::variant<MiracleError, vk::raii::Framebuffer> Device::createFramebuffer(
		const vk::FramebufferCreateInfo& createInfo
	) const {
		try {
			return m_device.createFramebuffer(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan framebuffer!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineFramebufferCreationError;
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

	std::variant<MiracleError, vk::raii::Semaphore> Device::createSemaphore(
		const vk::SemaphoreCreateInfo& createInfo
	) const {
		try {
			return m_device.createSemaphore(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan semaphore!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineSemaphoreCreationError;
		}
	}

	std::variant<MiracleError, vk::raii::Fence> Device::createFence(
		const vk::FenceCreateInfo& createInfo
	) const {
		try {
			return m_device.createFence(createInfo);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to create Vulkan fence!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineFenceCreationError;
		}
	}

	std::optional<MiracleError> Device::waitForFences(
		const vk::ArrayProxy<const vk::Fence>& fences,
		bool waitForAll,
		uint64_t timeout
	) const {
		try {
			auto result = m_device.waitForFences(fences, waitForAll, timeout);

			if (result == vk::Result::eTimeout) {
				Logger::warning("Timeout on wait for Vulkan fences!");
			}
		}
		catch (const std::exception& e) {
			Logger::error("Failed to wait for Vulkan fences!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineSyncError;
		}
		
		return std::nullopt;
	}

	std::optional<MiracleError> Device::resetFences(
		const vk::ArrayProxy<const vk::Fence>& fences
	) const {
		try {
			m_device.resetFences(fences);
		}
		catch (const std::exception& e) {
			Logger::error("Failed to reset Vulkan fences!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineFenceResetError;
		}

		return std::nullopt;
	}

	std::optional<MiracleError> Device::waitIdle() const {
		try {
			m_device.waitIdle();
		}
		catch (const std::exception& e) {
			Logger::error("Failed to wait for Vulkan device to finish execution!");
			Logger::error(e.what());
			return MiracleError::VulkanGraphicsEngineDeviceExecutionWaitError;
		}

		return std::nullopt;
	}

	void Device::refreshSupportDetails() {
		m_supportDetails = std::move(
			PhysicalDeviceSelector::queryDeviceSupport(m_physicalDevice, m_surface.getRawSurface())
		);
	}
}
