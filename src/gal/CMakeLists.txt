add_library(gal
    "command_buffer.h"
    "commands.cpp"
    "commands.h"
    "objects.cpp"
    "objects.h"
    "gal.h")
target_link_libraries(gal PUBLIC glm)

add_subdirectory(opengl)