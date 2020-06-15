target_sources(gfx_engine 
 PRIVATE 
    "image_loader.cpp"
    "image_loader.h"
    "model_loader.cpp"
    "model_loader.h"
    "resource_gal.h"
    "resource_manager_gal.cpp"
    "resource_manager_gal.h"
    "resource_manager.cpp"
    "resource_manager.h"
    "resource.cpp"
    "resource.h")
target_link_libraries(gfx_engine PRIVATE tinyobjloader)
target_link_libraries(gfx_engine PRIVATE stb)

add_executable(resource_tests
    "model_loader.cpp"
    "model_loader.h"
    "model_loader_unittest.cpp")
target_link_libraries(resource_tests PRIVATE glm)
target_link_libraries(resource_tests PRIVATE gtest_main)
target_link_libraries(resource_tests PRIVATE tinyobjloader)
target_link_libraries(resource_tests PRIVATE stb)
target_include_directories(resource_tests PRIVATE "${SRC_INCLUDE_DIR}")

add_custom_command(TARGET resource_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/test.obj" "$<TARGET_FILE_DIR:resource_tests>/test.obj")

add_test(NAME "ResourceTests" COMMAND resource_tests 
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:resource_tests>")
