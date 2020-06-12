add_library(platform
    "input_manager.cpp"
    "input_manager.h"
    "platform.cpp"
    "platform.h"
    "window.h"
    "window.cpp")
target_include_directories(platform PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(glfw)