#pragma once

namespace Miracle {
	enum class MiracleError : int {
		// No error
		NoError                                             = 0x00000000,
								                  
		// Window errors		                  
		WindowInitializationError                           = 0x00010000,
		WindowCreationError,

		// Vulkan graphics engine errors
		VulkanGraphicsEngineInstanceCreationError           = 0x00020000,
		VulkanGraphicsEngineDebugMessengerCreationError,
		VulkanGraphicsEngineSurfaceCreationError,
		VulkanGraphicsEnginePhysicalDeviceEnumerationError,
		VulkanGraphicsEngineNoPhysicalDevicesSupportedError,
		VulkanGraphicsEngineDeviceCreationError,
		VulkanGraphicsEngineSwapchainCreationError,
		VulkanGraphicsEngineSwapchainImagesRetrievalError,
		VulkanGraphicsEngineImageViewCreationError
	};
}
