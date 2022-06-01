cmake_minimum_required(VERSION 3.16)

project("Miracle")

# Options
option(BUILD_SHARED_LIBS ON)
option(MIRACLE_BUILD_DEMO_PROJECTS OFF)

# Main project
add_subdirectory("Miracle")

# Resources
add_subdirectory("Shaders")

# Demo projects
if(MIRACLE_BUILD_DEMO_PROJECTS)
	add_subdirectory("Demos/Demo1")
endif()
