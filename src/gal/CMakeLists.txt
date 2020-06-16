target_sources(gfx_engine
  PRIVATE
    "command.h"
    "gal_buffer.h"
    "gal.h"
    "object.cpp"
    "object.h"
    "platform.cpp"
    "platform.h")

add_subdirectory(opengl)