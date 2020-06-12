#include "../window.h"
#include "window_glfw.h"

#include <GL/glew.h>

#include <memory>

namespace window {

WindowImpl::WindowImpl(int width, int height, const std::string& title) {
  // glfwSetErrorCallback([](int err_code, const char* desc) {
  //   std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  // });
  if (!glfwInit()) {
    throw WindowInitException("WindowInitException: Failed to init GLFW.");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    throw WindowInitException("WindowInitException: Failed to create GLFW window.");
  }
  width_ = width;
  height_ = height;
  title_ = title;

  glfwMakeContextCurrent(glfw_window_);

  // TODO(colintan): Is glewExperiment needed?
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    throw WindowInitException("WindowInitException: Failed to init GLEW.");
  }

  // TODO(colintan): Do this somewhere else
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  initialized_ = true;
}

WindowImpl::~WindowImpl() {
  if (initialized_) {
    glfwDestroyWindow(glfw_window_);
    glfwTerminate();
  }
}

void WindowImpl::Tick() {
  glfwSwapBuffers(glfw_window_);
  glfwPollEvents();
}

bool WindowImpl::ShouldClose() {
  return glfwWindowShouldClose(glfw_window_);
}

Window::Window(int width, int height, const std::string& title) {
  impl_ = std::make_unique<WindowImpl>(width, height, title);
}

Window::~Window() {
  impl_.reset();
}

void Window::Tick() {
  impl_->Tick();
}

bool Window::ShouldClose() {
  return impl_->ShouldClose();
}

} // namespace