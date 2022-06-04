cmake_minimum_required(VERSION 3.12)

project("Miracle")

# Options
option(BUILD_SHARED_LIBS ON)
option(MIRACLE_BUILD_DEMO_PROJECTS OFF)

# Use top-level binary output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out/bin")

# Main project
add_subdirectory("Miracle")

# Resources
add_subdirectory("Shaders")

# Demo projects
if(MIRACLE_BUILD_DEMO_PROJECTS)
	add_subdirectory("Demos/Demo1")
endif()
