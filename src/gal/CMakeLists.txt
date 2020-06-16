target_sources(gfx_engine
  PRIVATE
    "command.h"
    "gal_buffer.h"
    "gal_pipeline.h"
    "gal_platform.cpp"
    "gal_platform.h"
    "gal_shader.h"
    "gal_texture.h"
    "object.cpp"
    "object.h"
    "texture_sampler.h")

add_subdirectory(object_impl)
add_subdirectory(opengl)