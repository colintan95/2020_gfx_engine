#include "platform/window.h"
#include "platform/glfw/window_glfw.h"

#include <GL/glew.h>

#include <memory>

namespace platform {

bool WindowImpl::Initialize(int width, int height, const std::string& title) {
  // glfwSetErrorCallback([](int err_code, const char* desc) {
  //   std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  // });
  if (!glfwInit()) {
    return false;
  }

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

  initialized_ = true;
  
  return true;
}

void WindowImpl::Destroy() {
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

// TODO(colintan): Figure out not to duplicate the code to set input_manager_
Window::Window(InputManager* input_manager)  {
  impl_ = std::make_unique<WindowImpl>();
  input_manager_ = input_manager;
}

Window::~Window() {
  input_manager_ = nullptr;
  impl_.reset();
}

bool Window::Initialize(int width, int height, const std::string& title) {
  return impl_->Initialize(width, height, title);
}

void Window::Destroy() {
  impl_->Destroy();
}

void Window::Tick() {
  impl_->Tick();
}

bool Window::ShouldClose() {
  return impl_->ShouldClose();
}

} // namespace