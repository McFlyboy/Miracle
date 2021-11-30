if(MIRACLE_BUILD_SHARED)
	set(LIBRARY_LINKAGE SHARED)
else()
	set(LIBRARY_LINKAGE STATIC)
endif()

add_library(Miracle ${LIBRARY_LINKAGE} "src/Miracle/MiracleApp.cpp" "include/Miracle/Miracle.hpp" "include/Miracle/components/Miracle/MiracleApp.hpp" "include/Miracle/components/PlatformDefinition.hpp" "include/Miracle/components/ApiDefinition.hpp" )

set_target_properties(
	Miracle
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_include_directories(Miracle PRIVATE "include/Miracle/components")

target_include_directories(
	Miracle
	INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	INTERFACE "$<INSTALL_INTERFACE:include>"
)

target_compile_definitions(Miracle PRIVATE "MIRACLE_USAGE_BUILD")

if(MIRACLE_BUILD_SHARED)
	target_compile_definitions(Miracle PUBLIC "MIRACLE_LINKAGE_SHARED")
endif()

target_precompile_headers(
	Miracle

	# Miracle
	PRIVATE "include/Miracle/components/PlatformDefinition.hpp"
	PRIVATE "include/Miracle/components/ApiDefinition.hpp"
)

install(TARGETS Miracle)
install(DIRECTORY "include/" DESTINATION "include")
