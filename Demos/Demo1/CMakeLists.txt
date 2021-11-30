add_executable(Demo1 "Demo.cpp")

set_target_properties(
	Demo1
	PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Demo1 PRIVATE Miracle)

if(MIRACLE_BUILD_SHARED)
	add_custom_command(
		TARGET Demo1
		POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Miracle> $<TARGET_FILE_DIR:Demo1>
	)
endif()

install(TARGETS Demo1)
