# Target definition
add_library(Miracle STATIC "src/Miracle/App.cpp" "src/Miracle/Infrastructure/Diagnostics/Spdlog/Logger.cpp" "src/Miracle/Application/EventDispatcher.cpp" "src/Miracle/EngineDependencies.cpp" "src/Miracle/Infrastructure/Framework/Glfw/MultimediaFramework.cpp" "src/Miracle/Infrastructure/View/Glfw/Window.cpp" "src/Miracle/Infrastructure/Input/Glfw/Keyboard.cpp" "src/Miracle/Application/TextInputService.cpp" "src/Miracle/Common/UnicodeConverter.cpp" "src/Miracle/Application/DeltaTimeService.cpp" "src/Miracle/Application/PerformanceCountingService.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/GraphicsContext.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/DeviceExplorer.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/Swapchain.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/GraphicsApi.cpp" "src/Miracle/Application/Graphics/Renderer.cpp" "src/Miracle/Infrastructure/Persistance/FileSystem/FileAccess.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/GraphicsPipeline.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/VertexBuffer.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/Vma.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/BufferUtilities.cpp" "src/Miracle/Infrastructure/Graphics/Vulkan/IndexBuffer.cpp" "src/Miracle/Infrastructure/Ecs/Entt/EcsContainer.cpp" "src/Miracle/Infrastructure/Ecs/Entt/Ecs.cpp" "src/Miracle/Application/SceneManager.cpp" "src/Miracle/Application/Models/Scene.cpp" "src/Miracle/Common/Math/Vector2f.cpp")

# Target properties
set_target_properties(
	Miracle
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED true
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Find 3rd-party packages
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(tinyfiledialogs CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(unofficial-vulkan-memory-allocator-hpp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# Link 3rd-party libraries
target_link_libraries(
	Miracle
	PRIVATE spdlog::spdlog
	PRIVATE glfw
	PRIVATE tinyfiledialogs::tinyfiledialogs
	PRIVATE Vulkan::Vulkan
	PRIVATE unofficial::VulkanMemoryAllocator-Hpp::VulkanMemoryAllocator-Hpp
	PRIVATE EnTT::EnTT
)

# Include directories
target_include_directories(Miracle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Miracle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Dependency targets
add_dependencies(Miracle Shaders)
