# Target definition
add_executable(Demo1 "Demo.cpp")

# Target properties
set_target_properties(
	Demo1
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED true
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Linking
target_link_libraries(Demo1 PRIVATE Miracle)
