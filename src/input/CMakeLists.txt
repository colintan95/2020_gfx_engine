add_library(input
    "input_manager.cpp"
    "input_manager.h")
target_include_directories(input PRIVATE "${SRC_INCLUDE_DIRS}")