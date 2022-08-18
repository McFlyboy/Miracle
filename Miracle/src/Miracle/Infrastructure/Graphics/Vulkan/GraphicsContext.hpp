#pragma once

#include <string_view>
#include <span>
#include <utility>
#include <array>
#include <vector>

#include <Miracle/Definitions.hpp>
#include <Miracle/Application/Graphics/IGraphicsContext.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"
#include "IContextTarget.hpp"
#include "DeviceInfo.hpp"
#include "SurfaceExtent.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsContext : public Application::IGraphicsContext {
	private:
		static inline auto s_validationLayerNames = std::array{ "VK_LAYER_KHRONOS_validation" };

		Application::ILogger& m_logger;
		IContextTarget& m_target;

		vk::raii::Context m_context;
		vk::raii::Instance m_instance = nullptr;
#ifdef MIRACLE_CONFIG_DEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger = nullptr;
#endif
		vk::raii::SurfaceKHR m_surface = nullptr;
		vk::raii::PhysicalDevice m_physicalDevice = nullptr;
		DeviceInfo m_deviceInfo;
		vk::raii::Device m_device = nullptr;
		vk::raii::Queue m_graphicsQueue = nullptr;
		vk::raii::Queue m_presentQueue = nullptr;
		vk::raii::CommandPool m_commandPool = nullptr;
		vk::raii::CommandBuffers m_commandBuffers = nullptr;
		std::vector<vk::raii::Semaphore> m_commandExecutionWaitSemaphores;
		std::vector<vk::raii::Semaphore> m_commandExecutionSignalSemaphores;
		std::vector<vk::raii::Fence> m_commandExecutionSignalFences;
		size_t m_currentCommandBufferIndex = 0;
	public:
		GraphicsContext(
			const std::string_view& appName,
			Application::ILogger& logger,
			IContextTarget& target
		);

		~GraphicsContext();

		virtual void recordCommands(const Application::Recording& recording) override;

		virtual void submitRecording() override;

		virtual void waitForDeviceIdle() override;

		inline const IContextTarget& getTarget() const { return m_target; }

		inline const vk::raii::SurfaceKHR& getSurface() const { return m_surface; }

		inline const DeviceInfo& getDeviceInfo() const { return m_deviceInfo; }

		inline const vk::raii::Device& getDevice() const { return m_device; }

		inline const vk::raii::Queue& getPresentQueue() const { return m_presentQueue; }

		inline const vk::raii::CommandBuffer& getCommandBuffer() const {
			return m_commandBuffers[m_currentCommandBufferIndex];
		}

		inline const vk::raii::Semaphore& getCommandExecutionWaitSemaphore() const {
			return m_commandExecutionWaitSemaphores[m_currentCommandBufferIndex];
		}

		inline const vk::raii::Semaphore& getCommandExecutionSignalSemaphore() const {
			return m_commandExecutionSignalSemaphores[m_currentCommandBufferIndex];
		}

		SurfaceExtent getCurrentSurfaceExtent() const;

		inline vk::SurfaceTransformFlagBitsKHR getCurrentSurfaceTransformation() const {
			return m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface).currentTransform;
		}

		inline void nextCommandBuffer() {
			++m_currentCommandBufferIndex %= m_commandBuffers.size();
		}

	private:
		vk::raii::Instance createInstance(const std::string_view& appName);

		void checkExtensionsAvailable(
			const std::span<const char*>& extensionNames
		) const;

#ifdef MIRACLE_CONFIG_DEBUG
		void checkValidationLayersAvailable(
			const std::span<const char*>& validationLayerNames
		) const;

		vk::raii::DebugUtilsMessengerEXT createDebugMessenger();

		vk::DebugUtilsMessengerCreateInfoEXT getDebugMessengerCreateInfo();

		static VKAPI_ATTR VkBool32 VKAPI_CALL logDebugMessage(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData
		);

#endif
		std::pair<vk::raii::PhysicalDevice, DeviceInfo> getMostOptimalPhysicalDevice() const;

		vk::raii::Device createDevice() const;

		vk::raii::CommandPool createCommandPool() const;

		vk::raii::CommandBuffers createCommandBuffers(size_t count) const;

		vk::raii::Fence createFence(bool preSignaled) const;

		vk::raii::Semaphore createSemaphore() const;
	};
}
