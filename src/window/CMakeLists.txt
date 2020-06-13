add_library(window
    "event.h"
    "window_internal.h"
    "window_manager.cpp"
    "window_manager.h"
    "window.cpp"
    "window.h")
target_include_directories(window PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(glfw)