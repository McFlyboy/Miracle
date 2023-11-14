#pragma once

#include <string_view>
#include <span>
#include <utility>
#include <array>
#include <vector>
#include <cstdint>

#include <Miracle/Definitions.hpp>
#include <Miracle/Application/Graphics/IGraphicsContext.hpp>
#include <Miracle/Application/ILogger.hpp>
#include "Vulkan.hpp"
#include "Vma.hpp"
#include "IContextTarget.hpp"
#include "DeviceInfo.hpp"
#include "SurfaceExtent.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class GraphicsContext : public Application::IGraphicsContext {
	private:
		static constexpr inline uint32_t s_vulkanApiVersion = VK_API_VERSION_1_1;
		static constexpr inline auto s_validationLayerNames = std::array{ "VK_LAYER_KHRONOS_validation" };

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
		std::vector<vk::raii::CommandBuffer> m_commandBuffers;
		std::vector<vk::raii::Semaphore> m_commandExecutionWaitSemaphores;
		std::vector<vk::raii::Semaphore> m_commandExecutionSignalSemaphores;
		std::vector<vk::raii::Fence> m_commandExecutionSignalFences;
		size_t m_currentCommandBufferIndex = 0;
		vma::Allocator m_allocator = nullptr;

	public:
		GraphicsContext(
			const std::string_view& appName,
			Application::ILogger& logger,
			IContextTarget& target
		);

		~GraphicsContext();

		virtual void setViewport(
			float x,
			float y,
			float width,
			float height
		) override;

		virtual void setScissor(
			int x,
			int y,
			unsigned int width,
			unsigned int height
		) override;

		virtual void draw(uint32_t vertexCount) override;

		virtual void drawIndexed(uint32_t indexCount) override;

		IContextTarget& getTarget() override { return m_target; }

		virtual void recordCommands(const std::function<void()>& recording) override;

		virtual void submitRecording() override;

		virtual void waitForDeviceIdle() override;

		const vk::raii::SurfaceKHR& getSurface() const { return m_surface; }

		const DeviceInfo& getDeviceInfo() const { return m_deviceInfo; }

		const vk::raii::Device& getDevice() const { return m_device; }

		const vk::raii::Queue& getGraphicsQueue() const { return m_graphicsQueue; }

		const vk::raii::Queue& getPresentQueue() const { return m_presentQueue; }

		const vk::raii::CommandBuffer& getCommandBuffer() const {
			return m_commandBuffers[m_currentCommandBufferIndex];
		}

		const vk::raii::Semaphore& getCommandExecutionWaitSemaphore() const {
			return m_commandExecutionWaitSemaphores[m_currentCommandBufferIndex];
		}

		const vk::raii::Semaphore& getCommandExecutionSignalSemaphore() const {
			return m_commandExecutionSignalSemaphores[m_currentCommandBufferIndex];
		}

		const vma::Allocator& getAllocator() const { return m_allocator; }

		SurfaceExtent getCurrentSurfaceExtent() const;

		vk::SurfaceTransformFlagBitsKHR getCurrentSurfaceTransformation() const {
			return m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface).currentTransform;
		}

		void nextCommandBuffer() {
			++m_currentCommandBufferIndex %= m_commandBuffers.size();
		}

		void recreateSemaphores();

	private:
		vk::raii::Instance createInstance(const std::string_view& appName);

		void checkExtensionsAvailable(
			const std::span<const char*>& extensionNames
		) const;

#ifdef MIRACLE_CONFIG_DEBUG
		void checkValidationLayersAvailable() const;

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

		std::vector<vk::raii::CommandBuffer> allocateCommandBuffers(size_t count) const;

		vk::raii::Fence createFence(bool preSignaled) const;

		vk::raii::Semaphore createSemaphore() const;

		vma::Allocator createAllocator() const;
	};
}
