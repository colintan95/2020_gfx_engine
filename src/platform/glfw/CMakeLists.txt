target_sources(platform
  PRIVATE
    "impl_creator_glfw.cpp"
    "impl_creator_glfw.h"
    "input_source_glfw.cpp"
    "input_source_glfw.h"
    "platform_glfw.cpp"
    "platform_glfw.h"
    "window_glfw.cpp"
    "window_glfw.h")
target_link_libraries(platform PRIVATE glew)
target_link_libraries(platform PRIVATE glfw)