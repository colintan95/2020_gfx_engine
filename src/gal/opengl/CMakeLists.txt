target_sources(gal
  PRIVATE
    "command_buffer_gl.cpp"
    "id_converter.cpp"
    "id_converter.h"
    "objects_gl.cpp")
target_link_libraries(gal PRIVATE glew)
target_link_libraries(gal PRIVATE OpenGL::GL)
target_link_libraries(gal PRIVATE OpenGL::GLU)