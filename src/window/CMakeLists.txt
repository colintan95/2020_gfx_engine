add_library(window
    "window.cpp"
    "window.h"
    "window_impl.h")
target_link_libraries(window PRIVATE glew)
target_link_libraries(window PRIVATE glfw)