target_sources(gfx_engine
  PRIVATE
    "command.h"
    "gal_buffer.h"
    "gal_object.h"
    "gal_shader.h"
    "gal_texture.h"
    "gal.h"
    "object.cpp"
    "object.h"
    "platform.cpp"
    "platform.h")

add_subdirectory(object_impl)
add_subdirectory(opengl)