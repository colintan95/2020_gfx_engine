target_sources(window
  PRIVATE
    "window_internal_glfw.cpp"
    "window_internal_glfw.h"
    "window_manager_impl_glfw.cpp"
    "window_manager_impl_glfw.h")
target_link_libraries(window PRIVATE glfw)