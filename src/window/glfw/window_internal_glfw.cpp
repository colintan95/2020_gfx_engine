#include "window/glfw/window_internal_glfw.h"

#include <memory>

namespace window {

bool WindowInternalGLFW::CreateWindow(int width, int height, const std::string& title) {
  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    return false;
  }
  width_ = width;
  height_ = height;
  title_ = title;
  return true;
}

void WindowInternalGLFW::DestroyWindow() {
  if (glfw_window_ != nullptr) {
    glfwDestroyWindow(glfw_window_);
  }
}
   
void WindowInternalGLFW::SwapBuffers() {

}

std::unique_ptr<WindowInternal> WindowInternal::Create() {
  return std::make_unique<WindowInternalGLFW>();
}

} // namespace