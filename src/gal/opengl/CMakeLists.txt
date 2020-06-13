target_sources(gal
  PRIVATE
    "command_gl.cpp"
    "id_converter.cpp"
    "id_converter.h"
    "object_gl.cpp"
    "platform_gl.cpp"
    "platform_gl.h")
target_link_libraries(gal PRIVATE glew)
target_link_libraries(gal PRIVATE OpenGL::GL)
target_link_libraries(gal PRIVATE OpenGL::GLU)