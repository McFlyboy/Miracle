add_executable(Demo1 "Demo.cpp")

set_target_properties(
	Demo1
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Demo1 Miracle)

install(TARGETS Demo1)
