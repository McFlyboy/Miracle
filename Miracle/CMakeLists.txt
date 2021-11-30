# Linkage definition
if(MIRACLE_BUILD_SHARED)
	set(LIBRARY_LINKAGE SHARED)
else()
	set(LIBRARY_LINKAGE STATIC)
endif()

# Project definition
add_library(Miracle ${LIBRARY_LINKAGE} "src/Miracle/MiracleApp.cpp" "include/Miracle/Miracle.hpp" "include/Miracle/components/Miracle/MiracleApp.hpp" "include/Miracle/components/PlatformDefinition.hpp" "include/Miracle/components/ApiDefinition.hpp" )

# Project properties
set_target_properties(
	Miracle
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(Miracle PRIVATE "include/Miracle/components")
target_include_directories(
	Miracle
	INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	INTERFACE "$<INSTALL_INTERFACE:include>"
)

# Usage macros
target_compile_definitions(Miracle PRIVATE "MIRACLE_USAGE_BUILD")
target_compile_definitions(Miracle INTERFACE "MIRACLE_USAGE_CONSUME")

# Linkage macro
if(MIRACLE_BUILD_SHARED)
	target_compile_definitions(Miracle PUBLIC "MIRACLE_LINKAGE_SHARED")
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
