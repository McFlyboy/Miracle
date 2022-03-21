# Target post-build command: Copy shaders to destination
function(copy_shaders INVOKER_TARGET SHADERS_DESTINATION)
	add_custom_command(
		TARGET ${INVOKER_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DESTINATION}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/Shaders/default.vert.spv" ${SHADERS_DESTINATION}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_BINARY_DIR}/Shaders/default.frag.spv" ${SHADERS_DESTINATION}
	)
endfunction()
