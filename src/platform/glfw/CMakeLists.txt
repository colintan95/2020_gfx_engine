target_sources(platform
  PRIVATE
    "window_glfw.cpp"
    "window_glfw.h")
target_link_libraries(platform PRIVATE glew)
target_link_libraries(platform PRIVATE glfw)