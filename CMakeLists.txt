cmake_minimum_required(VERSION 3.13)
project(gfx_engine)

# Set paths for third party libraries
set(GLEW_INCLUDE_DIRS "third_party/glew-2.1.0/include")
set(GLFW_INCLUDE_DIRS "third_party/glfw-3.3.2/include")
set(GLM_INCLUDE_DIRS "third_party/glm-0.9.9.8/include")
set(GLEW_STATIC_LIBS "third_party/glew-2.1.0/lib/Release/x64")
set(GLFW_STATIC_LIBS "third_party/glfw-3.3.2/lib-vc2019")

# Find OpenGL
find_package(OpenGL)
if(OPENGL_FOUND)
  message(STATUS "Found OpenGL")
else()
  message(FATAL_ERROR "Unable to find OpenGL")
endif()

add_executable(gfx_engine src/main.cpp)

set_property(TARGET gfx_engine PROPERTY CXX_STANDARD 17)

target_include_directories(gfx_engine PUBLIC "${GLM_INCLUDE_DIRS}")
target_include_directories(gfx_engine PUBLIC "${GLEW_INCLUDE_DIRS}")
target_include_directories(gfx_engine PUBLIC "${GLFW_INCLUDE_DIRS}")
target_include_directories(gfx_engine PUBLIC "${OPENGL_INCLUDE_DIRS}")

# TODO(colintan): See if I can use a relative path rather than an absolute one
target_link_libraries(gfx_engine PUBLIC "${CMAKE_SOURCE_DIR}/${GLEW_STATIC_LIBS}/glew32.lib")
target_link_libraries(gfx_engine PUBLIC "${CMAKE_SOURCE_DIR}/${GLFW_STATIC_LIBS}/glfw3.lib")
target_link_libraries(gfx_engine PUBLIC OpenGL::GL)
target_link_libraries(gfx_engine PUBLIC OpenGL::GLU)