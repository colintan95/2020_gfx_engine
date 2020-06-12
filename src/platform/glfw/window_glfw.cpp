#include "platform/window.h"
#include "platform/glfw/window_glfw.h"

#include <GL/glew.h>

#include <memory>

namespace window {

bool WindowImplGLFW::Initialize() {
  // glfwSetErrorCallback([](int err_code, const char* desc) {
  //   std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  // });
  return true;
}

void WindowImplGLFW::Cleanup() {
  if (has_window_) {
    glfwDestroyWindow(glfw_window_);
  }
}

bool WindowImplGLFW::CreateWindow(int width, int height, const std::string& title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    return false;
  }
  width_ = width;
  height_ = height;
  title_ = title;

  glfwMakeContextCurrent(glfw_window_);

  // TODO(colintan): Is glewExperiment needed?
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    return false;
  }

  // TODO(colintan): Do this somewhere else
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  has_window_ = true;
  
  return true;
}

void WindowImplGLFW::Tick() {
  glfwSwapBuffers(glfw_window_);
}

bool WindowImplGLFW::ShouldClose() {
  return glfwWindowShouldClose(glfw_window_);
}

} // namespace