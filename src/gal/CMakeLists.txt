add_library(gal
    "command.h"
    "gal.h"
    "object.cpp"
    "object.h"
    "platform.cpp"
    "platform.h")
target_link_libraries(gal PUBLIC glm)
target_include_directories(gal PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(opengl)