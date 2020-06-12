add_library(gal
    "commands.h"
    "gal.h"
    "objects.cpp"
    "objects.h")
target_link_libraries(gal PUBLIC glm)
target_include_directories(gal PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(opengl)