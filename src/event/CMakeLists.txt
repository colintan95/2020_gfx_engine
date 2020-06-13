add_library(event
    "event.cpp"
    "event.h")
    target_include_directories(event PRIVATE "${SRC_INCLUDE_DIR}")