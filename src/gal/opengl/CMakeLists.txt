target_sources(gfx_engine
  PRIVATE
    "command_gl.cpp"
    "gal_platform_gl.cpp"
    "gal_platform_gl.h"
    "id_converter.cpp"
    "id_converter.h"
    "object_gl.cpp")
target_link_libraries(gfx_engine PRIVATE glew)
target_link_libraries(gfx_engine PRIVATE OpenGL::GL)
target_link_libraries(gfx_engine PRIVATE OpenGL::GLU)

add_subdirectory(object_impl)