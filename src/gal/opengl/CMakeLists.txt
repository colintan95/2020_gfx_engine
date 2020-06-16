target_sources(gfx_engine
  PRIVATE
    "gal_command_buffer_gl.cpp"
    "gal_platform_gl.cpp"
    "gal_platform_gl.h")
target_link_libraries(gfx_engine PRIVATE glew)
target_link_libraries(gfx_engine PRIVATE OpenGL::GL)
target_link_libraries(gfx_engine PRIVATE OpenGL::GLU)

add_subdirectory(object_impl)