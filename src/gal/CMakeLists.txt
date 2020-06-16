target_sources(gfx_engine
  PRIVATE
    "command.h"
    "gal_buffer_impl.h"
    "gal_buffer.h"
    "gal_shader_impl.h"
    "gal_shader.h"
    "gal_texture_impl.h"
    "gal_texture.h"
    "gal.h"
    "object.cpp"
    "object.h"
    "platform.cpp"
    "platform.h")

add_subdirectory(opengl)