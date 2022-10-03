# Find 3rd-party packages
find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

# Shader source filenames
set(
	SOURCE_FILENAMES
		"Default.vert"
		"Default.frag"
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
set(CMAKE_CURRENT_BINARY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Assets/Shaders")

# Add path to binary filenames
list(
	TRANSFORM BINARY_FILENAMES
	PREPEND "${CMAKE_CURRENT_BINARY_DIR}/"
)

# Target definition
add_custom_target(
	MiracleShaders
	DEPENDS ${BINARY_FILENAMES}
)

# Target build commands
foreach(SOURCE_FILENAME ${SOURCE_FILENAMES})
	add_custom_command(
		OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILENAME}${BINARY_SUFFIX}"
		COMMAND ${glslc_executable} "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILENAME}" -o "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILENAME}${BINARY_SUFFIX}"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILENAME}"
		COMMENT "Building shader module ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILENAME}${BINARY_SUFFIX}"
	)
endforeach()
