target_sources(gfx_engine
  PRIVATE
    "event_consumer.cpp"
    "event_consumer.h"
    "window_impl.h"
    "window_manager.cpp"
    "window_manager.h"
    "window_surface.h"
    "window.cpp"
    "window.h")

add_subdirectory(glfw)
add_subdirectory(vulkan)