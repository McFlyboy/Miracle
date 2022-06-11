# Target definition
add_library(Miracle STATIC "src/Miracle/App.cpp" "src/Miracle/Diagnostics/Logger.cpp" "src/Miracle/View/Implementations/Window.cpp" "src/Miracle/Input/Devices/Implementations/Keyboard.cpp" "src/Miracle/EngineDependencies.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsEngine.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Instance.cpp" "src/Miracle/Graphics/Implementations/Vulkan/PhysicalDeviceSelector.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Device.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Surface.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Swapchain.cpp" "src/Miracle/Io/ResourceLoader.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsPipeline.cpp" "src/Miracle/Graphics/Implementations/Vulkan/RenderPass.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsQueue.cpp" "src/Miracle/Graphics/Implementations/Vulkan/FrameInFlight.cpp" "src/Miracle/Graphics/Implementations/Vulkan/PresentQueue.cpp" "src/Miracle/Graphics/Implementations/Vulkan/VertexBuffer.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Buffer.cpp" "src/Miracle/Graphics/Implementations/Vulkan/IndexBuffer.cpp")

# Target properties
set_target_properties(
	Miracle
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED true
)

# Find 3rd-party packages
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Link 3rd-party libraries
target_link_libraries(
	Miracle
	PRIVATE spdlog::spdlog
	PRIVATE glfw
	PRIVATE Vulkan::Vulkan
)

# Include directories
target_include_directories(Miracle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Miracle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Target resource dependencies
add_dependencies(Miracle Shaders)
