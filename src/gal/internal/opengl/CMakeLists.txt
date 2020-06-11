target_sources(gal
  PRIVATE
    "interface.cc")
target_link_libraries(gal PRIVATE glew)
target_link_libraries(gfx_engine PRIVATE OpenGL::GL)
target_link_libraries(gfx_engine PRIVATE OpenGL::GLU)