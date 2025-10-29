#include "GraphicsContext.hpp"

#include <cstring>
#include <exception>
#include <limits>
#include <format>

#include <Miracle/Environment.hpp>
#include "DeviceExplorer.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsContext::GraphicsContext(
		const std::string_view& appName,
		Application::ILogger& logger,
		IContextTarget& target
	) :
		m_logger(logger),
		m_target(target),
		m_instance(createInstance(appName)),
#ifdef MIRACLE_CONFIG_DEBUG
		m_debugMessenger(createDebugMessenger()),
#endif
		m_surface(target.createVulkanSurface(m_instance))
	{
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

		m_transferQueue = m_device.getQueue(
			m_deviceInfo.queueFamilyIndices.transferFamilyIndex.value(),
			0
		);

		m_graphicsCommandPool = createCommandPool(m_deviceInfo.queueFamilyIndices.graphicsFamilyIndex.value());
		m_transferCommandPool = createCommandPool(m_deviceInfo.queueFamilyIndices.transferFamilyIndex.value());
		m_graphicsCommandBuffers = allocateCommandBuffers(m_graphicsCommandPool, 2);
		m_transferCommandBuffer = std::move(
			allocateCommandBuffers(m_transferCommandPool, 1)
				.front()
		);

		m_renderingCompletedFences.reserve(m_graphicsCommandBuffers.size());
		m_renderingCompletedSemaphores.reserve(m_graphicsCommandBuffers.size());
		m_presentCompletedSemaphores.reserve(m_graphicsCommandBuffers.size());

		for (size_t i = 0; i < m_graphicsCommandBuffers.size(); i++) {
			m_renderingCompletedFences.push_back(createFence(true));
			m_renderingCompletedSemaphores.push_back(createSemaphore());
			m_presentCompletedSemaphores.push_back(createSemaphore());
		}

		m_allocator = createAllocator();

		m_logger.info("Vulkan graphics context created");
	}

	GraphicsContext::~GraphicsContext() {
		m_logger.info("Destroying Vulkan graphics context...");

		vmaDestroyAllocator(m_allocator);
	}

	void GraphicsContext::setViewport(
		float x,
		float y,
		float width,
		float height
	) {
		getGraphicsCommandBuffer().setViewport(
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
		getGraphicsCommandBuffer().setScissor(
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

	void GraphicsContext::setDepthTestEnabled(bool enabled) {
		getGraphicsCommandBuffer().setDepthTestEnable(enabled);
	}

	void GraphicsContext::setDepthWriteEnabled(bool enabled) {
		getGraphicsCommandBuffer().setDepthWriteEnable(enabled);
	}

	void GraphicsContext::draw(uint32_t vertexCount) {
		getGraphicsCommandBuffer().draw(vertexCount, 1, 0, 0);
	}

	void GraphicsContext::drawIndexed(uint32_t indexCount) {
		getGraphicsCommandBuffer().drawIndexed(indexCount, 1, 0, 0, 0);
	}

	void GraphicsContext::recordGraphicsCommands(const std::function<void()>& recording) {
		auto result = m_device.waitForFences(
			*m_renderingCompletedFences[m_currentGraphicsCommandBufferIndex],
			true,
			std::numeric_limits<uint64_t>::max()
		);

		if (result == vk::Result::eTimeout) [[unlikely]] {
			m_logger.warning("Timed out on waiting for Vulkan fence");
		}

		m_graphicsCommandBuffers[m_currentGraphicsCommandBufferIndex].begin(
			vk::CommandBufferBeginInfo{
				.flags            = {},
				.pInheritanceInfo = {}
			}
		);

		recording();

		m_graphicsCommandBuffers[m_currentGraphicsCommandBufferIndex].end();
	}

	void GraphicsContext::recordTransferCommands(const std::function<void()>& recording) {
		m_transferCommandBuffer.reset();
		m_transferCommandBuffer.begin(
			vk::CommandBufferBeginInfo{
				.flags            = {},
				.pInheritanceInfo = {}
			}
		);

		recording();

		m_transferCommandBuffer.end();
	}

	void GraphicsContext::submitGraphicsRecording() {
		vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;

		m_device.resetFences(*m_renderingCompletedFences[m_currentGraphicsCommandBufferIndex]);

		m_graphicsQueue.submit(
			vk::SubmitInfo{
				.waitSemaphoreCount   = 1,
				.pWaitSemaphores      = &*m_presentCompletedSemaphores[m_currentGraphicsCommandBufferIndex],
				.pWaitDstStageMask    = &waitStage,
				.commandBufferCount   = 1,
				.pCommandBuffers      = &*m_graphicsCommandBuffers[m_currentGraphicsCommandBufferIndex],
				.signalSemaphoreCount = 1,
				.pSignalSemaphores    = &*m_renderingCompletedSemaphores[m_currentGraphicsCommandBufferIndex]
			},
			* m_renderingCompletedFences[m_currentGraphicsCommandBufferIndex]
		);
	}

	void GraphicsContext::submitTransferRecording() {
		m_transferQueue.submit(
			vk::SubmitInfo{
				.waitSemaphoreCount   = 0,
				.pWaitSemaphores      = nullptr,
				.pWaitDstStageMask    = {},
				.commandBufferCount   = 1,
				.pCommandBuffers      = &*m_transferCommandBuffer,
				.signalSemaphoreCount = 0,
				.pSignalSemaphores    = nullptr
			}
		);
	}

	void GraphicsContext::waitForDeviceIdle() {
		m_device.waitIdle();
	}

	SurfaceExtent GraphicsContext::getCurrentSurfaceExtent() const {
		auto surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface);

		return surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()
			? SurfaceExtent{
				.extent = surfaceCapabilities.currentExtent
			}
			: SurfaceExtent{
				.extent = SurfaceExtentBounds{
					.minExtent = surfaceCapabilities.minImageExtent,
					.maxExtent = surfaceCapabilities.maxImageExtent
				}
			};
	}

	void GraphicsContext::recreatePresentCompletedSemaphores() {
		m_presentCompletedSemaphores.clear();

		for (size_t i = 0; i < m_graphicsCommandBuffers.size(); i++) {
			m_presentCompletedSemaphores.push_back(createSemaphore());
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
		
		if constexpr (Environment::getCurrentPlatform() == Platform::platformMacos) {
			extensionNames.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		}

		checkExtensionsAvailable(extensionNames);

		try {
			return m_context.createInstance(
				vk::InstanceCreateInfo{
#ifdef MIRACLE_CONFIG_DEBUG
					.pNext                   = &debugMessengerCreateInfo,
#endif
#ifdef MIRACLE_PLATFORM_MACOS
					.flags                   = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
#endif
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

	void GraphicsContext::checkExtensionsAvailable(const std::span<const char*>& extensionNames) const {
		bool allExtensionsFound = true;

		auto extensionPropertiesList = m_context.enumerateInstanceExtensionProperties();

		for (auto& extensionName : extensionNames) {
			bool extensionFound = false;

			for (auto& extensionProperties : extensionPropertiesList) {
				if (std::strcmp(extensionName, extensionProperties.extensionName) == 0) {
					extensionFound = true;
					break;
				}
			}

			if (extensionFound) continue;

			m_logger.error(std::format("Vulkan extension missing: {}", extensionName));
			allExtensionsFound = false;
		}

		if (!allExtensionsFound) {
			throw Application::GraphicsContextErrors::FunctionalityNotSupportedError();
		}
	}

#ifdef MIRACLE_CONFIG_DEBUG
	void GraphicsContext::checkValidationLayersAvailable() const {
		bool allLayersFound = true;

		auto layerPropertiesList = m_context.enumerateInstanceLayerProperties();

		for (auto& validationLayerName : s_validationLayerNames) {
			bool layerFound = false;

			for (auto& layerProperties : layerPropertiesList) {
				if (std::strcmp(validationLayerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (layerFound) continue;

			m_logger.error(std::format("Vulkan validation layer missing: {}", validationLayerName));
			allLayersFound = false;
		}

		if (!allLayersFound) throw Application::GraphicsContextErrors::DebugToolsUnavailableError();
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
			.flags           = {},
			.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			.messageType     = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
				| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
				| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			.pfnUserCallback = logDebugMessage,
			.pUserData       = this
		};
	}

	vk::Bool32 VKAPI_PTR GraphicsContext::logDebugMessage(
		vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT messageType,
		const vk::DebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData
	) {
		switch (messageSeverity) {
		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
			reinterpret_cast<GraphicsContext*>(userData)
				->m_logger.error(callbackData->pMessage);
			break;

		case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
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

		auto extensionNames = std::array{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME
		};

		auto extendedDynamicStateFeatures = vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{
			.extendedDynamicState = true
		};

		try {
			return m_physicalDevice.createDevice(
				vk::DeviceCreateInfo{
					.pNext                   = &extendedDynamicStateFeatures,
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

	vk::raii::CommandPool GraphicsContext::createCommandPool(uint32_t queueFamilyIndex) const {
		try {
			return m_device.createCommandPool(
				vk::CommandPoolCreateInfo{
					.flags            = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
					.queueFamilyIndex = queueFamilyIndex
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

	std::vector<vk::raii::CommandBuffer> GraphicsContext::allocateCommandBuffers(
		vk::raii::CommandPool& commandPool,
		uint32_t count
	) const {
		try {
			return m_device.allocateCommandBuffers(
				vk::CommandBufferAllocateInfo{
					.commandPool        = *commandPool,
					.level              = vk::CommandBufferLevel::ePrimary,
					.commandBufferCount = count
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

	VmaAllocator GraphicsContext::createAllocator() const {
		auto createInfo = VmaAllocatorCreateInfo{
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
		};

		VmaAllocator allocator;

		auto result = vmaCreateAllocator(&createInfo, &allocator);

		if (result != VK_SUCCESS) {
			m_logger.error(std::format("Failed to create Vulkan memory allocator for context.\n{}", "vmaCreateAllocator"));
			throw Application::GraphicsContextErrors::CreationError();
		}

		return allocator;
	}
}
