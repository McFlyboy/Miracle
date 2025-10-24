cmake_minimum_required(VERSION 3.21)

project("Miracle")

# Options
option(MIRACLE_BUILD_DEMO_TARGETS false)

# Use top-level binary output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/bin")

# Dependency targets
add_subdirectory("Shaders")

# Main target
add_subdirectory("Miracle")

# Demo targets
if(MIRACLE_BUILD_DEMO_TARGETS)
	add_subdirectory("Demos/Demo1")
endif()
