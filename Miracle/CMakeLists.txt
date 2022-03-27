# Linkage definition
if(MIRACLE_BUILD_SHARED)
	set(LIBRARY_TYPE SHARED)
else()
	set(LIBRARY_TYPE STATIC)
endif()

# Target definition
add_library(Miracle ${LIBRARY_TYPE} "src/Miracle/MiracleApp.cpp" "include/Miracle/Miracle.hpp" "include/Miracle/components/Miracle/MiracleApp.hpp" "include/Miracle/components/PlatformDefinition.hpp" "include/Miracle/components/ApiDefinition.hpp" "include/Miracle/components/Miracle/Diagnostics/Logger.hpp" "src/Miracle/Diagnostics/Logger.cpp" "include/Miracle/components/Miracle/View/IWindow.hpp" "src/Miracle/View/Implementations/Window.hpp" "src/Miracle/View/Implementations/Window.cpp" "include/Miracle/components/Miracle/MiracleAppBuilder.hpp" "src/Miracle/MiracleAppBuilder.cpp" "include/Miracle/components/Miracle/Input/Devices/IKeyboard.hpp" "src/Miracle/Input/Devices/Implementations/Keyboard.hpp" "src/Miracle/Input/Devices/Implementations/Keyboard.cpp" "src/Miracle/EngineDependencies.hpp" "src/Miracle/EngineDependencies.cpp"  "src/Miracle/MiracleError.hpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsEngine.hpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsEngine.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Vulkan.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Instance.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Instance.cpp" "src/Miracle/Graphics/Implementations/Vulkan/PhysicalDeviceSelector.hpp" "src/Miracle/Graphics/Implementations/Vulkan/PhysicalDeviceSelector.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Device.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Device.cpp" "src/Miracle/Graphics/Implementations/Vulkan/DeviceSupportDetails.hpp" "src/Miracle/Graphics/Implementations/Vulkan/ISurfaceTarget.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Surface.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Surface.cpp" "src/Miracle/Graphics/Implementations/Vulkan/Swapchain.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Swapchain.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsQueue.hpp" "src/Miracle/Graphics/Implementations/Vulkan/PresentQueue.hpp" "src/Miracle/Graphics/Implementations/Vulkan/Queue.hpp" "src/Miracle/Io/ResourceLoader.hpp" "src/Miracle/Io/ResourceLoader.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsPipeline.hpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsPipeline.cpp" "src/Miracle/Graphics/Implementations/Vulkan/RenderPass.hpp" "src/Miracle/Graphics/Implementations/Vulkan/RenderPass.cpp" "src/Miracle/Graphics/Implementations/Vulkan/GraphicsQueue.cpp" "src/Miracle/Graphics/IGraphicsEngine.hpp" "src/Miracle/Graphics/Implementations/Vulkan/EngineSyncObjects.hpp" "src/Miracle/Graphics/Implementations/Vulkan/EngineSyncObjects.cpp" "src/Miracle/Graphics/Implementations/Vulkan/PresentQueue.cpp")

# Target properties
set_target_properties(
	Miracle
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
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
target_include_directories(Miracle PRIVATE "src")
target_include_directories(
	Miracle
	PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	INTERFACE "$<INSTALL_INTERFACE:include>"
)

# Usage macros
target_compile_definitions(Miracle PRIVATE "MIRACLE_USAGE_BUILD")
target_compile_definitions(Miracle INTERFACE "MIRACLE_USAGE_CONSUME")

# Linkage macro
if(MIRACLE_BUILD_SHARED)
	target_compile_definitions(Miracle PUBLIC "MIRACLE_LINKAGE_DYNAMIC")
endif()

# Pre-compiled headers
target_precompile_headers(
	Miracle

	# Miracle
	PRIVATE "include/Miracle/components/PlatformDefinition.hpp"
	PRIVATE "include/Miracle/components/ApiDefinition.hpp"
)

# Target resource dependencies
add_dependencies(Miracle Shaders)

# Include resource related scripts
include("${PROJECT_SOURCE_DIR}/Shaders/CopyShaders.cmake")

# Copy shader binaries to resources
copy_shaders(Miracle "$<TARGET_FILE_DIR:Miracle>/resources/shaders")

# Install rules
install(TARGETS Miracle)
install(DIRECTORY "include/" DESTINATION "include")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources/" DESTINATION "bin/resources")
