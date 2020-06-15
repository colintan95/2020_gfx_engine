target_sources(gfx_engine
  PRIVATE
    "command.h"
    "gal.h"
    "object.cpp"
    "object.h"
    "platform.cpp"
    "platform.h")

add_subdirectory(opengl)