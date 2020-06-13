#include "window/glfw/window_glfw.h"

#include <iostream>
#include <memory>

namespace window {
namespace internal {

bool WindowGLFW::CreateWindow(int width, int height, const std::string& title) {
  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    return false;
  }
  width_ = width;
  height_ = height;
  title_ = title;

  glfwMakeContextCurrent(glfw_window_);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    return false;
  }

  glfwSetKeyCallback(glfw_window_, KeyboardCallback);

  return true;
}

void WindowGLFW::DestroyWindow() {
  if (glfw_window_ != nullptr) {
    glfwDestroyWindow(glfw_window_);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void WindowGLFW::Tick() {
  glfwPollEvents();
}
   
void WindowGLFW::SwapBuffers() {
  glfwSwapBuffers(glfw_window_);
}

std::optional<event::Event> WindowGLFW::ConsumeEvent() {
  return event_store_.ConsumeEvent(glfw_window_);
}

bool WindowGLFW::ShouldClose() {
  return glfwWindowShouldClose(glfw_window_);
}

EventStore WindowGLFW::event_store_;

void WindowGLFW::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  event_store_.KeyboardCallback(window, key, scancode, action, mods);
}

std::unique_ptr<Window> Window::Create() {
  return std::make_unique<WindowGLFW>();
}

} // namespace
} // namespace