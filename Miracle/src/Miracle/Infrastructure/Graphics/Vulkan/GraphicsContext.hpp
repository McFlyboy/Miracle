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
		static constexpr uint32_t s_vulkanApiVersion = VK_API_VERSION_1_1;
		static constexpr auto s_validationLayerNames = std::array{ "VK_LAYER_KHRONOS_validation" };

		Application::ILogger& m_logger;
		IContextTarget& m_target;

		vk::raii::Context m_context;
		vk::raii::Instance m_instance;
#ifdef MIRACLE_CONFIG_DEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger;
#endif
		vk::raii::SurfaceKHR m_surface;
		vk::raii::PhysicalDevice m_physicalDevice = nullptr;
		DeviceInfo m_deviceInfo;
		vk::raii::Device m_device = nullptr;
		vk::raii::Queue m_graphicsQueue = nullptr;
		vk::raii::Queue m_presentQueue = nullptr;
		vk::raii::Queue m_transferQueue = nullptr;
		vk::raii::CommandPool m_graphicsCommandPool = nullptr;
		vk::raii::CommandPool m_transferCommandPool = nullptr;
		std::vector<vk::raii::CommandBuffer> m_graphicsCommandBuffers;
		vk::raii::CommandBuffer m_transferCommandBuffer = nullptr;
		std::vector<vk::raii::Fence> m_renderingCompletedFences;
		std::vector<vk::raii::Semaphore> m_renderingCompletedSemaphores;
		std::vector<vk::raii::Semaphore> m_presentCompletedSemaphores;
		size_t m_currentGraphicsCommandBufferIndex = 0;
		vma::Allocator m_allocator;

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

		virtual void recordGraphicsCommands(const std::function<void()>& recording) override;

		void recordTransferCommands(const std::function<void()>& recording);

		virtual void submitGraphicsRecording() override;

		void submitTransferRecording();

		virtual void waitForDeviceIdle() override;

		const vk::raii::SurfaceKHR& getSurface() const { return m_surface; }

		const DeviceInfo& getDeviceInfo() const { return m_deviceInfo; }

		const vk::raii::Device& getDevice() const { return m_device; }

		const vk::raii::Queue& getGraphicsQueue() const { return m_graphicsQueue; }

		const vk::raii::Queue& getPresentQueue() const { return m_presentQueue; }

		const vk::raii::Queue& getTransferQueue() const { return m_transferQueue; }

		const vk::raii::CommandBuffer& getGraphicsCommandBuffer() const {
			return m_graphicsCommandBuffers[m_currentGraphicsCommandBufferIndex];
		}

		const vk::raii::CommandBuffer& getTransferCommandBuffer() const {
			return m_transferCommandBuffer;
		}

		const vk::raii::Semaphore& getGraphicsCommandExecutionCompletedSemaphore() const {
			return m_renderingCompletedSemaphores[m_currentGraphicsCommandBufferIndex];
		}

		const vk::raii::Semaphore& getGraphicsCommandPresentCompletedSemaphore() const {
			return m_presentCompletedSemaphores[m_currentGraphicsCommandBufferIndex];
		}

		const vma::Allocator& getAllocator() const { return m_allocator; }

		SurfaceExtent getCurrentSurfaceExtent() const;

		vk::SurfaceTransformFlagBitsKHR getCurrentSurfaceTransformation() const {
			return m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface).currentTransform;
		}

		void nextGraphicsCommandBuffer() {
			++m_currentGraphicsCommandBufferIndex %= m_graphicsCommandBuffers.size();
		}

		void recreatePresentCompletedSemaphores();

	private:
		vk::raii::Instance createInstance(const std::string_view& appName);

		void checkExtensionsAvailable(const std::span<const char*>& extensionNames) const;

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

		vk::raii::CommandPool createCommandPool(uint32_t queueFamilyIndex) const;

		std::vector<vk::raii::CommandBuffer> allocateCommandBuffers(
			vk::raii::CommandPool& commandPool,
			uint32_t count
		) const;

		vk::raii::Fence createFence(bool preSignaled) const;

		vk::raii::Semaphore createSemaphore() const;

		vma::Allocator createAllocator() const;
	};
}
