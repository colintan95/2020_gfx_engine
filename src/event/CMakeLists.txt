add_library(event
    "event_handler.cpp"
    "event_handler.h"
    "event_manager.cpp"
    "event_manager.h"
    "event.cpp"
    "event.h")
target_include_directories(event PRIVATE "${SRC_INCLUDE_DIR}")