# Target definition
add_executable(Demo2 "Demo.cpp")

# Target properties
set_target_properties(
	Demo2
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED true
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Linking
target_link_libraries(Demo2 PRIVATE Miracle)
