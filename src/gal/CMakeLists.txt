add_library(gal
    "commands.h"
    "gal.h"
    "objects.cpp"
    "objects.h")
target_link_libraries(gal PUBLIC glm)

add_subdirectory(opengl)