add_library(resource
    "image_loader.cpp"
    "image_loader.h"
    "model_loader.cpp"
    "model_loader.h")
target_link_libraries(resource PUBLIC glm)
target_link_libraries(resource PRIVATE tinyobjloader)
target_link_libraries(resource PRIVATE stb)
target_include_directories(resource PRIVATE "${SRC_INCLUDE_DIR}")

add_executable(resource_tests
    "model_loader_unittest.cpp")
target_link_libraries(resource_tests PRIVATE resource)
target_link_libraries(resource_tests PRIVATE glm)
target_link_libraries(resource_tests PRIVATE gtest_main)
target_include_directories(resource_tests PRIVATE "${SRC_INCLUDE_DIR}")

add_custom_command(TARGET resource_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/test.obj" "$<TARGET_FILE_DIR:resource_tests>/test.obj")

add_test(NAME "ResourceTests" COMMAND resource_tests 
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:resource_tests>")
