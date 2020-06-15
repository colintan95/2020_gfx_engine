target_sources(gfx_engine
  PRIVATE
    "command_gl.cpp"
    "id_converter.cpp"
    "id_converter.h"
    "object_gl.cpp"
    "platform_gl.cpp"
    "platform_gl.h")
target_link_libraries(gfx_engine PRIVATE glew)
target_link_libraries(gfx_engine PRIVATE OpenGL::GL)
target_link_libraries(gfx_engine PRIVATE OpenGL::GLU)