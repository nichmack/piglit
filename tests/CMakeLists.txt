include_directories(
	${piglit_BINARY_DIR}/tests/util
	${piglit_SOURCE_DIR}/tests/util
)

add_subdirectory (util)
add_subdirectory (bugs)
add_subdirectory (fbo)
add_subdirectory (general)
add_subdirectory (glx)
add_subdirectory (glslparsertest)
add_subdirectory (hiz)
add_subdirectory (asmparsertest)
add_subdirectory (security)
add_subdirectory (shaders)
add_subdirectory (texturing)
add_subdirectory (spec)
add_subdirectory (fast_color_clear)

if (NOT APPLE)
	# glean relies on AGL which is deprecated/broken on recent Mac OS X
	# versions
	add_subdirectory (glean)
endif ()

IF(OPENGL_egl_LIBRARY)
	add_subdirectory (egl)
ENDIF(OPENGL_egl_LIBRARY)

IF(PIGLIT_BUILD_CL_TESTS)
	add_subdirectory (cl)
ENDIF(PIGLIT_BUILD_CL_TESTS)
