#include "GraphicsContext.hpp"

#include <cstring>
#include <exception>
#include <limits>
#include <format>

#include "DeviceExplorer.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsContext::GraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger,
		IContextTarget& target
	) :
		m_logger(logger),
		m_target(target)
	{
		m_instance = createInstance(appName);
#ifdef MIRACLE_CONFIG_DEBUG
		m_debugMessenger = createDebugMessenger();
#endif
		m_surface = target.createVulkanSurface(m_instance);

		auto [physicalDevice, deviceInfo] = getMostOptimalPhysicalDevice();

		m_physicalDevice = std::move(physicalDevice);
		m_deviceInfo = std::move(deviceInfo);
		m_device = createDevice();

		m_graphicsQueue = m_device.getQueue(
			m_deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value(),
			0
		);

		m_presentQueue = m_device.getQueue(
			m_deviceInfo.queueFamilyIndices.presentFamilyIndex.value(),
			0
		);

		m_commandPool = createCommandPool();
		m_commandBuffers = allocateCommandBuffers(2);

		m_commandExecutionWaitSemaphores.reserve(m_commandBuffers.size());
		m_commandExecutionSignalSemaphores.reserve(m_commandBuffers.size());
		m_commandExecutionSignalFences.reserve(m_commandBuffers.size());

		for (auto& commandBuffer : m_commandBuffers) {
			m_commandExecutionWaitSemaphores.push_back(createSemaphore());
			m_commandExecutionSignalSemaphores.push_back(createSemaphore());
			m_commandExecutionSignalFences.push_back(createFence(true));
		}

		m_allocator = createAllocator();

		m_logger.info("Vulkan graphics context created");
	}

	GraphicsContext::~GraphicsContext() {
		m_logger.info("Destroying Vulkan graphics context...");

		m_allocator.destroy();
	}

	void GraphicsContext::setViewport(
		float x,
		float y,
		float width,
		float height
	) {
		getCommandBuffer().setViewport(
			0,
			vk::Viewport{
				.x        = x,
				.y        = y,
				.width    = width,
				.height   = height,
				.minDepth = 0.0f,
				.maxDepth = 1.0f
			}
		);
	}

	void GraphicsContext::setScissor(
		int x,
		int y,
		unsigned int width,
		unsigned int height
	) {
		getCommandBuffer().setScissor(
			0,
			vk::Rect2D{
				.offset = vk::Offset2D{
					.x = x,
					.y = y
				},
				.extent = vk::Extent2D{
					.width  = width,
					.height = height
				}
			}
		);
	}

	void GraphicsContext::draw(uint32_t vertexCount) {
		getCommandBuffer().draw(vertexCount, 1, 0, 0);
	}

	void GraphicsContext::drawIndexed(uint32_t indexCount) {
		getCommandBuffer().drawIndexed(indexCount, 1, 0, 0, 0);
	}

	void GraphicsContext::recordCommands(const std::function<void()>& recording) {
		auto result = m_device.waitForFences(
			*m_commandExecutionSignalFences[m_currentCommandBufferIndex],
			true,
			std::numeric_limits<uint64_t>::max()
		);

		if (result == vk::Result::eTimeout) [[unlikely]] {
			m_logger.warning("Timed out on waiting for Vulkan fence");
		}

		m_commandBuffers[m_currentCommandBufferIndex].reset();
		m_commandBuffers[m_currentCommandBufferIndex].begin(
			vk::CommandBufferBeginInfo{
				.flags            = {},
				.pInheritanceInfo = {}
			}
		);

		recording();

		m_commandBuffers[m_currentCommandBufferIndex].end();
	}

	void GraphicsContext::submitRecording() {
		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		m_device.resetFences(*m_commandExecutionSignalFences[m_currentCommandBufferIndex]);

		m_graphicsQueue.submit(
			vk::SubmitInfo{
				.waitSemaphoreCount   = 1,
				.pWaitSemaphores      = &*m_commandExecutionWaitSemaphores[m_currentCommandBufferIndex],
				.pWaitDstStageMask    = &waitStage,
				.commandBufferCount   = 1,
				.pCommandBuffers      = &*m_commandBuffers[m_currentCommandBufferIndex],
				.signalSemaphoreCount = 1,
				.pSignalSemaphores    = &*m_commandExecutionSignalSemaphores[m_currentCommandBufferIndex]
			},
			*m_commandExecutionSignalFences[m_currentCommandBufferIndex]
		);
	}

	void GraphicsContext::waitForDeviceIdle() {
		m_device.waitIdle();
	}

	SurfaceExtent GraphicsContext::getCurrentSurfaceExtent() const {
		auto surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface);

		return SurfaceExtent{
			.extent    = surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()
				? std::optional(surfaceCapabilities.currentExtent)
				: std::nullopt,
			.minExtent = surfaceCapabilities.minImageExtent,
			.maxExtent = surfaceCapabilities.maxImageExtent
		};
	}

	void GraphicsContext::recreateSemaphores() {
		m_commandExecutionWaitSemaphores.clear();

		for (auto& commandBuffer : m_commandBuffers) {
			m_commandExecutionWaitSemaphores.push_back(createSemaphore());
		}
	}

	vk::raii::Instance GraphicsContext::createInstance(const std::string_view& appName) {
		auto appInfo = vk::ApplicationInfo{
			.pApplicationName   = appName.data(),
			.applicationVersion = VK_MAKE_VERSION(0, 0, 0),
			.pEngineName        = "Miracle",
			.engineVersion      = VK_MAKE_VERSION(0, 0, 0),
			.apiVersion         = s_vulkanApiVersion
		};

		auto extensionNames = std::vector<const char*>();

		auto extensionNamesRequiredByTarget = m_target.getRequiredVulkanExtensionNames();

		for (auto& extensionName : extensionNamesRequiredByTarget) {
			extensionNames.push_back(extensionName);
		}

#ifdef MIRACLE_CONFIG_DEBUG
		extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		checkValidationLayersAvailable();

		auto debugMessengerCreateInfo = getDebugMessengerCreateInfo();
#endif

		checkExtensionsAvailable(extensionNames);

		try {
			return m_context.createInstance(
				vk::InstanceCreateInfo{
#ifdef MIRACLE_CONFIG_DEBUG
					.pNext                   = &debugMessengerCreateInfo,
#endif
					.flags                   = {},
					.pApplicationInfo        = &appInfo,
#ifdef MIRACLE_CONFIG_DEBUG
					.enabledLayerCount       = static_cast<uint32_t>(s_validationLayerNames.size()),
					.ppEnabledLayerNames     = s_validationLayerNames.data(),
#endif
					.enabledExtensionCount   = static_cast<uint32_t>(extensionNames.size()),
					.ppEnabledExtensionNames = extensionNames.data()
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format(
					"Failed to create Vulkan instance.\n{}",
					e.what()
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	void GraphicsContext::checkExtensionsAvailable(
		const std::span<const char*>& extensionNames
	) const {
		bool allExtensionsFound = true;

		auto extensionsProperties = m_context.enumerateInstanceExtensionProperties();

		for (auto& extensionName : extensionNames) {
			bool extensionFound = false;

			for (auto& extensionProperties : extensionsProperties) {
				if (std::strcmp(extensionName, extensionProperties.extensionName) == 0) {
					extensionFound = true;
					break;
				}
			}

			if (extensionFound) {
				continue;
			}

			m_logger.error(std::format("Vulkan extension missing: {}", extensionName));
			allExtensionsFound = false;
		}

		if (allExtensionsFound) {
			return;
		}

		throw Application::GraphicsContextErrors::FunctionalityNotSupportedError();
	}

#ifdef MIRACLE_CONFIG_DEBUG
	void GraphicsContext::checkValidationLayersAvailable() const {
		bool allLayersFound = true;

		auto layersProperties = m_context.enumerateInstanceLayerProperties();

		for (auto& validationLayerName : s_validationLayerNames) {
			bool layerFound = false;

			for (auto& layerProperties : layersProperties) {
				if (std::strcmp(validationLayerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (layerFound) {
				continue;
			}

			m_logger.error(std::format("Vulkan validation layer missing: {}", validationLayerName));
			allLayersFound = false;
		}

		if (allLayersFound) {
			return;
		}

		throw Application::GraphicsContextErrors::DebugToolsUnavailableError();
	}

	vk::raii::DebugUtilsMessengerEXT GraphicsContext::createDebugMessenger() {
		try {
			return m_instance.createDebugUtilsMessengerEXT(getDebugMessengerCreateInfo());
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format(
					"Failed to create Vulkan debug messenger.\n{}",
					e.what()
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vk::DebugUtilsMessengerCreateInfoEXT GraphicsContext::getDebugMessengerCreateInfo() {
		return vk::DebugUtilsMessengerCreateInfoEXT{
			.flags = {},
			.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
				| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
				| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			.pfnUserCallback = logDebugMessage,
			.pUserData = this
		};
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsContext::logDebugMessage(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	) {
		switch (messageSeverity) {
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.error(callbackData->pMessage);
			break;

		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.warning(callbackData->pMessage);
			break;

		default:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.info(callbackData->pMessage);
		}

		return false;
	}
#endif

	std::pair<vk::raii::PhysicalDevice, DeviceInfo> GraphicsContext::getMostOptimalPhysicalDevice() const {
		auto allDevices = m_instance.enumeratePhysicalDevices();
		
		if (allDevices.empty()) {
			m_logger.error("No Vulkan physical devices found");
			throw Application::GraphicsContextErrors::GraphicsDeviceNotFoundError();
		}

		m_logger.info(
			std::format("Vulkan physical devices found: {}", allDevices.size())
		);

		auto supportedDevices = std::vector<std::pair<vk::raii::PhysicalDevice*, DeviceInfo>>();
		supportedDevices.reserve(allDevices.size());

		for (auto& device : allDevices) {
			auto deviceInfo = DeviceExplorer::getDeviceInfo(device, m_surface);

			if (!DeviceExplorer::isDeviceSupported(deviceInfo)) continue;

			supportedDevices.emplace_back(&device, std::move(deviceInfo));
		}

		if (supportedDevices.empty()) {
			m_logger.error("No Vulkan physical devices supported");
			throw Application::GraphicsContextErrors::NoGraphicsDeviceSupportedError();
		}

		m_logger.info(
			std::format("Supported Vulkan physical devices: {}", supportedDevices.size())
		);

		size_t selectedDeviceIndex = 0;

		for (size_t i = 1; i < supportedDevices.size(); i++) {
			auto& selectedDeviceInfo = supportedDevices[selectedDeviceIndex].second;
			auto& deviceInfo = supportedDevices[i].second;

			bool isSelectedDeviceDiscrete = selectedDeviceInfo.type == vk::PhysicalDeviceType::eDiscreteGpu;
			bool isDeviceDiscrete = deviceInfo.type == vk::PhysicalDeviceType::eDiscreteGpu;

			if (
				(isDeviceDiscrete && !isSelectedDeviceDiscrete)
					|| (
						(isDeviceDiscrete == isSelectedDeviceDiscrete)
							&& (deviceInfo.deviceLocalMemorySize > selectedDeviceInfo.deviceLocalMemorySize)
					)
			) {
				selectedDeviceIndex = i;
			}
		}

		auto& [selectedDevice, deviceInfo] = supportedDevices[selectedDeviceIndex];

		m_logger.info(
			std::format(
				"Selected Vulkan device: {} [{}]",
				deviceInfo.name,
				vk::to_string(deviceInfo.type)
			)
		);

		return std::pair(std::move(*selectedDevice), std::move(deviceInfo));
	}

	vk::raii::Device GraphicsContext::createDevice() const {
		float queuePriority = 1.0f;

		auto uniqueQueueFamilyIndices = m_deviceInfo.queueFamilyIndices.createSetOfAllUniqueIndices();

		auto deviceQueueCreateInfos = std::vector<vk::DeviceQueueCreateInfo>();
		deviceQueueCreateInfos.reserve(uniqueQueueFamilyIndices.size());

		for (auto& queueFamilyIndex : uniqueQueueFamilyIndices) {
			deviceQueueCreateInfos.push_back(
				vk::DeviceQueueCreateInfo{
					.flags            = {},
					.queueFamilyIndex = queueFamilyIndex,
					.queueCount       = 1,
					.pQueuePriorities = &queuePriority
				}
			);
		}

		auto extensionNames = std::array{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		try {
			return m_physicalDevice.createDevice(
				vk::DeviceCreateInfo{
					.flags                   = {},
					.queueCreateInfoCount    = static_cast<uint32_t>(deviceQueueCreateInfos.size()),
					.pQueueCreateInfos       = deviceQueueCreateInfos.data(),
#ifdef MIRACLE_CONFIG_DEBUG
					.enabledLayerCount = static_cast<uint32_t>(s_validationLayerNames.size()),
					.ppEnabledLayerNames = s_validationLayerNames.data(),
#endif
					.enabledExtensionCount   = static_cast<uint32_t>(extensionNames.size()),
					.ppEnabledExtensionNames = extensionNames.data(),
					.pEnabledFeatures        = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(std::format("Failed to create Vulkan device.\n{}", e.what()));
			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vk::raii::CommandPool GraphicsContext::createCommandPool() const {
		try {
			return m_device.createCommandPool(
				vk::CommandPoolCreateInfo{
					.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
					.queueFamilyIndex = m_deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value()
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan command pool for context.\n{}", e.what())
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	std::vector<vk::raii::CommandBuffer> GraphicsContext::allocateCommandBuffers(size_t count) const {
		try {
			return m_device.allocateCommandBuffers(
				vk::CommandBufferAllocateInfo{
					.commandPool        = *m_commandPool,
					.level              = vk::CommandBufferLevel::ePrimary,
					.commandBufferCount = static_cast<uint32_t>(count)
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan command buffers for context.\n{}", e.what())
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vk::raii::Fence GraphicsContext::createFence(bool preSignaled) const {
		try {
			return m_device.createFence(
				vk::FenceCreateInfo{
					.flags = preSignaled
						? vk::FenceCreateFlagBits::eSignaled
						: vk::FenceCreateFlagBits()
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(
				std::format("Failed to create Vulkan fence for context.\n{}", e.what())
			);

			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vk::raii::Semaphore GraphicsContext::createSemaphore() const {
		try {
			return m_device.createSemaphore(
				vk::SemaphoreCreateInfo{
					.flags = {}
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(std::format("Failed to create Vulkan semaphore for context.\n{}", e.what()));
			throw Application::GraphicsContextErrors::CreationError();
		}
	}

	vma::Allocator GraphicsContext::createAllocator() const {
		try {
			return vma::createAllocator(
				vma::AllocatorCreateInfo{
					.flags							= {},
					.physicalDevice					= *m_physicalDevice,
					.device							= *m_device,
					.preferredLargeHeapBlockSize	= {},
					.pAllocationCallbacks			= {},
					.pDeviceMemoryCallbacks			= {},
					.pHeapSizeLimit					= {},
					.pVulkanFunctions				= {},
					.instance						= *m_instance,
					.vulkanApiVersion				= s_vulkanApiVersion,
					.pTypeExternalMemoryHandleTypes	= {}
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(std::format("Failed to create Vulkan memory allocator for context.\n{}", e.what()));
			throw Application::GraphicsContextErrors::CreationError();
		}
	}
}
