add_library(Miracle "src/MiracleApp.cpp" "include/Miracle/Miracle.hpp" "include/Miracle/components/MiracleApp.hpp")

set_target_properties(
	Miracle
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_include_directories(Miracle PRIVATE "include/Miracle/components")
target_include_directories(Miracle INTERFACE "include")
