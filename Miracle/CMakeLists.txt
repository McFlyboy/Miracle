# Linkage definition
if(MIRACLE_BUILD_SHARED)
	set(LIBRARY_TYPE SHARED)
else()
	set(LIBRARY_TYPE STATIC)
endif()

# Project definition
add_library(Miracle ${LIBRARY_TYPE} "src/Miracle/MiracleApp.cpp" "include/Miracle/Miracle.hpp" "include/Miracle/components/Miracle/MiracleApp.hpp" "include/Miracle/components/PlatformDefinition.hpp" "include/Miracle/components/ApiDefinition.hpp" "include/Miracle/components/Miracle/Diagnostics/Logger.hpp" "src/Miracle/Diagnostics/Logger.cpp" "include/Miracle/components/Miracle/View/IWindow.hpp" "src/Miracle/View/Implementations/Window.hpp" "src/Miracle/View/Implementations/Window.cpp" "include/Miracle/components/Miracle/MiracleAppBuilder.hpp" "src/Miracle/MiracleAppBuilder.cpp")

# Project properties
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

# Link 3rd-party libraries
target_link_libraries(
	Miracle
	PRIVATE spdlog::spdlog
	PRIVATE glfw
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

# Install rules
install(TARGETS Miracle)
install(DIRECTORY "include/" DESTINATION "include")
