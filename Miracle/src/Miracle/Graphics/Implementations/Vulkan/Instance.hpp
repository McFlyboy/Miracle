#pragma once

#include <array>
#include <variant>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"
#include "ISurfaceTarget.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Instance {
	private:
		const ISurfaceTarget& m_surfaceTarget;

		vk::raii::Instance m_instance = nullptr;
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger = nullptr;
#endif

	public:
		Instance(const vk::raii::Context& context, const ISurfaceTarget& surfaceTarget);

		std::variant<MiracleError, vk::raii::SurfaceKHR> createSurface() const;

		std::variant<MiracleError, std::vector<vk::raii::PhysicalDevice>> enumeratePhysicalDevices() const;

		inline const ISurfaceTarget& getSurfaceTarget() const { return m_surfaceTarget; }

		static inline auto getValidationLayers() {
			return
#ifndef NDEBUG
				std::array<const char*, 1>{ "VK_LAYER_KHRONOS_validation" }
#else
				std::array<const char*, 0>{}
#endif
			;
		}

	private:
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData
		);
	};
}
