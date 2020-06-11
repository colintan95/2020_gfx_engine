add_library(resource STATIC
    "model_loader.cpp"
    "model_loader.h")
target_link_libraries(resource PUBLIC glm)
target_link_libraries(resource PUBLIC tinyobjloader)

add_executable(resource_tests
    "model_loader_unittest.cpp")
target_link_libraries(resource_tests resource)
target_include_directories(resource_tests PRIVATE "${GTEST_INCLUDE_DIR}")
target_link_libraries(resource_tests gtest_main)

add_test(NAME "ResourceTests" COMMAND resource_tests)