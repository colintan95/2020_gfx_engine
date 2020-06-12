target_sources(window
  PRIVATE
    "window_glfw.cpp"
    "window_glfw.h")
target_link_libraries(window PRIVATE glew)
target_link_libraries(window PRIVATE glfw)