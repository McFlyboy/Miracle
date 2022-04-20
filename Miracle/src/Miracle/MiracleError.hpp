#pragma once

namespace Miracle {
	enum class MiracleError : int {
		// No error (should not be used as success value unless casted to type int first)
		NoError                                                  = 0x00000000,
		
		// Window errors		                  
		WindowInitializationError                                = 0x00010000,
		WindowCreationError,

		// Resource errors
		ResourceFileReadError                                    = 0x00020000,

		// Vulkan graphics engine errors
		VulkanGraphicsEngineInstanceCreationError                = 0x00030000,
		VulkanGraphicsEngineDebugMessengerCreationError,
		VulkanGraphicsEngineSurfaceCreationError,
		VulkanGraphicsEnginePhysicalDeviceEnumerationError,
		VulkanGraphicsEngineNoPhysicalDevicesSupportedError,
		VulkanGraphicsEngineDeviceCreationError,
		VulkanGraphicsEngineCommandPoolCreationError,
		VulkanGraphicsEngineCommandBufferAllocationError,
		VulkanGraphicsEngineCommandBufferResetError,
		VulkanGraphicsEngineCommandRecordBeginError,
		VulkanGraphicsEngineCommandRecordEndError,
		VulkanGraphicsEngineSwapchainCreationError,
		VulkanGraphicsEngineSwapchainImagesRetrievalError,
		VulkanGraphicsEngineImageViewCreationError,
		VulkanGraphicsEngineRenderPassCreationError,
		VulkanGraphicsEngineFramebufferCreationError,
		VulkanGraphicsEngineShaderModuleCreationError,
		VulkanGraphicsEnginePipelineLayoutCreationError,
		VulkanGraphicsEngineGraphicsPipelineCreationError,
		VulkanGraphicsEngineSemaphoreCreationError,
		VulkanGraphicsEngineFenceCreationError,
		VulkanGraphicsEngineBufferCreationError,
		VulkanGraphicsEngineMemoryAllocationError,
		VulkanGraphicsEngineSyncError,
		VulkanGraphicsEngineFenceResetError,
		VulkanGraphicsEngineImageAcquisitionError,
		VulkanGraphicsEngineCommandBufferSubmissionError,
		VulkanGraphicsEnginePresentError,
		VulkanGraphicsEngineDeviceExecutionWaitError,
		VulkanGraphicsEngineSwapchainOutOfDateError,
		VulkanGraphicsEngineBufferMemoryTypeSelectionError
	};
}
