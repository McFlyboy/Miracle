cmake_minimum_required(VERSION 3.16)

project("Miracle")

# Options
option(MIRACLE_BUILD_DEMO_PROJECTS OFF)
option(MIRACLE_BUILD_SHARED ON)

# Main project
add_subdirectory("Miracle")

# Demo projects
if(MIRACLE_BUILD_DEMO_PROJECTS)
	add_subdirectory("Demos/Demo1")
endif()
