add_library(window
    "window.h")
target_include_directories(window PRIVATE "${SRC_INCLUDE_DIR}")

add_subdirectory(glfw)