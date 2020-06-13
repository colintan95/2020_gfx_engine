target_sources(window
  PRIVATE
    "event_store.cpp"
    "event_store.h"
    "window_glfw.cpp"
    "window_glfw.h"
    "window_manager_glfw.cpp"
    "window_manager_glfw.h")
target_link_libraries(window PRIVATE glfw)