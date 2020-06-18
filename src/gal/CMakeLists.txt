target_sources(gfx_engine
  PRIVATE
    # "commands.h"
    # "gal_buffer.h"
    # "gal_command_buffer.h"
    # "gal_pipeline.h"
    "gal_platform.cpp"
    "gal_platform.h"
#     "gal_shader.h"
#     "gal_texture.h"
#     "gal_texture_sampler.h"
)

add_subdirectory(object_impl)

if(GFXAPI_GL)
  add_subdirectory(opengl)
elseif(GFXAPI_VK)
  add_subdirectory(vulkan)
endif()