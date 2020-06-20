target_sources(gfx_engine
  PRIVATE
    "gal_command_buffer_vk.cpp"
    "gal_platform_vk.cpp"
    "gal_platform_vk.h"
)

add_subdirectory(object_impl)