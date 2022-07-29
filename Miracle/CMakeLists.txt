# Target definition
add_library(Miracle STATIC "src/Miracle/App.cpp" "src/Miracle/Infrastructure/Diagnostics/Spdlog/Logger.cpp" "src/Miracle/Application/EventDispatcher.cpp" "src/Miracle/EngineDependencies.cpp" "src/Miracle/Infrastructure/Framework/Glfw/MultimediaFramework.cpp" "src/Miracle/Infrastructure/View/Glfw/Window.cpp" "src/Miracle/Infrastructure/Input/Glfw/Keyboard.cpp" "src/Miracle/Application/TextInputService.cpp" "src/Miracle/Common/UnicodeConverter.cpp")

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
find_package(tinyfiledialogs CONFIG REQUIRED)

# Link 3rd-party libraries
target_link_libraries(
	Miracle
	PRIVATE spdlog::spdlog
	PRIVATE glfw
	PRIVATE tinyfiledialogs::tinyfiledialogs
)

# Include directories
target_include_directories(Miracle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Miracle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
