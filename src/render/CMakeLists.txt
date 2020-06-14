add_library(render
    "model.h"
    "render.h"
    "renderer.cpp"
    "renderer.h")
target_include_directories(render PRIVATE "${SRC_INCLUDE_DIR}")