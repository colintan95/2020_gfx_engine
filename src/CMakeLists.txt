add_executable(gfx_engine 
    "application.cpp"
    "application.h"
    "main.cpp")

# Internal modules
target_link_libraries(gfx_engine PRIVATE event)
target_link_libraries(gfx_engine PRIVATE gal)
target_link_libraries(gfx_engine PRIVATE logging)
target_link_libraries(gfx_engine PRIVATE render)
target_link_libraries(gfx_engine PRIVATE resource)
target_link_libraries(gfx_engine PRIVATE scene)
target_link_libraries(gfx_engine PRIVATE window)

# External libs
target_link_libraries(gfx_engine PRIVATE glm)
target_link_libraries(gfx_engine PRIVATE glfw)

target_include_directories(gfx_engine PRIVATE "${SRC_INCLUDE_DIRS}")

# TODO(colintan): Don't do this
# In the binary folder, create a symlink to the assets folder
add_custom_command(TARGET gfx_engine POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E create_symlink "${CMAKE_SOURCE_DIR}/assets" 
    "$<TARGET_FILE_DIR:gfx_engine>/assets")

add_subdirectory(event)
add_subdirectory(gal)
add_subdirectory(logging)
add_subdirectory(render)
add_subdirectory(resource)
add_subdirectory(scene)
add_subdirectory(window)