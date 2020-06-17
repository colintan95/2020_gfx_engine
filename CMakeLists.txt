cmake_minimum_required(VERSION 3.15)
project(gfx_engine)

set(CMAKE_CXX_STANDARD 17)

message(${CMAKE_SYSTEM_NAME})

set(PLATFORM_GFXAPI "OpenGL")
set(PLATFORM_WINDOW "GLFW")

if ("${PLATFORM_GFXAPI}" STREQUAL "OpenGL")
    add_compile_definitions(GFXAPI_GL)
elseif ("${PLATFORM_GFXAPI}" STREQUAL "Vulkan")
    add_compile_definitions(GFXAPI_VK)
endif()

if ("${PLATFORM_WINDOW}" STREQUAL "GLFW")
    add_compile_definitions(WINDOW_GLFW)
endif()

# OpenGL
find_package(OpenGL)
if(OPENGL_FOUND)
  message(STATUS "Found OpenGL")
else()
  message(FATAL_ERROR "Unable to find OpenGL")
endif()

# GLEW
add_library(glew STATIC 
    "${CMAKE_SOURCE_DIR}/third_party/glew-2.1.0/src/glew.c"
    "${CMAKE_SOURCE_DIR}/third_party/glew-2.1.0/include/GL/glew.h")
target_include_directories(glew PUBLIC 
    "${CMAKE_SOURCE_DIR}/third_party/glew-2.1.0/include")
# GLEW_STATIC is defined for both compilation and interface compilation as we need it when
# we generate the library and when other files include its header
# TODO(colintan): Consider using the dll lib instead
set_target_properties(glew PROPERTIES COMPILE_OPTIONS "-DGLEW_STATIC")
set_target_properties(glew PROPERTIES INTERFACE_COMPILE_OPTIONS "-DGLEW_STATIC")

# GLFW
add_library(glfw STATIC IMPORTED)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION
    "${CMAKE_SOURCE_DIR}/third_party/glfw-3.3.2/lib-vc2019/glfw3.lib")
set_property(TARGET glfw PROPERTY INTERFACE_INCLUDE_DIRECTORIES 
    "${CMAKE_SOURCE_DIR}/third_party/glfw-3.3.2/include")

# GLM
add_library(glm INTERFACE IMPORTED)
set_property(TARGET glm PROPERTY INTERFACE_INCLUDE_DIRECTORIES 
    "${CMAKE_SOURCE_DIR}/third_party/glm-0.9.9.8/include")

# tinyobjloader
add_library(tinyobjloader INTERFACE IMPORTED)
set_property(TARGET tinyobjloader PROPERTY INTERFACE_INCLUDE_DIRECTORIES 
    "${CMAKE_SOURCE_DIR}/third_party/tinyobjloader-2.0.0/include")

# STB
add_library(stb INTERFACE IMPORTED)
set_property(TARGET stb PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    "${CMAKE_SOURCE_DIR}/third_party/stb")

# Add googletest
add_subdirectory("third_party/googletest")
set(GTEST_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/third_party/googletest/googletest/include")

enable_testing()

set(SRC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory(src)