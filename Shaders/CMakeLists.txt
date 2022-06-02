# Find 3rd-party packages
find_package(Vulkan REQUIRED)

# Compiler
set(SPIRV_COMPILER ${Vulkan_GLSLC_EXECUTABLE})

# Shader source filenames
set(
	SOURCE_FILENAMES
		"default.vert"
		"default.frag"
)

# Shader binary file suffix
set(BINARY_SUFFIX ".spv")

# Shader binary filenames
list(
	TRANSFORM SOURCE_FILENAMES
	APPEND ${BINARY_SUFFIX}
	OUTPUT_VARIABLE BINARY_FILENAMES
)

# Set binary output directory
set(CMAKE_CURRENT_BINARY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders")

# Add path to binary filenames
list(
	TRANSFORM BINARY_FILENAMES
	PREPEND "${CMAKE_CURRENT_BINARY_DIR}/"
)

# Target definition
add_custom_target(
	Shaders
	DEPENDS ${BINARY_FILENAMES}
)

# Target build commands
foreach(SOURCE_FILENAME ${SOURCE_FILENAMES})
	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILENAME}${BINARY_SUFFIX}"
		COMMAND ${SPIRV_COMPILER} "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILENAME}" -o "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILENAME}${BINARY_SUFFIX}"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILENAME}"
	)
endforeach()
