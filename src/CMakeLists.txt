add_executable(gfx_engine 
    "main.cpp")

# External libs
target_link_libraries(gfx_engine PRIVATE glm)
target_link_libraries(gfx_engine PRIVATE glfw)

target_include_directories(gfx_engine PRIVATE "${SRC_INCLUDE_DIRS}")

if(GFXAPI_VK)
  target_link_libraries(gfx_engine PRIVATE Vulkan::Vulkan)
endif()

# TODO(colintan): Don't do this
# In the binary folder, create a symlink to the assets folder
add_custom_command(TARGET gfx_engine POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E create_symlink "${CMAKE_SOURCE_DIR}/assets" 
    "$<TARGET_FILE_DIR:gfx_engine>/assets")

add_subdirectory(event)
add_subdirectory(gal)
add_subdirectory(render)
add_subdirectory(resource)
add_subdirectory(scene)
add_subdirectory(window)