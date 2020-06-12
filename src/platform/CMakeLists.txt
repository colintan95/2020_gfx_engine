add_library(platform
    "input_manager.cpp"
    "input_manager.h"
    "window.h")
target_include_directories(platform PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(glfw)