add_library(render
    "model.h"
    "render.h"
    "renderer.cpp"
    "renderer.h"
    "resource_manager.cpp"
    "resource_manager.h")
target_include_directories(render PRIVATE "${SRC_INCLUDE_DIR}")