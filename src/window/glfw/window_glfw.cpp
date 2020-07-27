#include "window/glfw/window_glfw.h"

#include <iostream>
#include <memory>
#include "window/window_surface.h"

namespace window {
namespace internal {

bool WindowImplGLFW::CreateWindow(int width, int height, const std::string& title) {

#if defined(GFXAPI_GL)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

#if defined(GFXAPI_VK)
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    return false;
  }
  width_ = width;
  height_ = height;
  title_ = title;

#if defined(GFXAPI_GL)
  glfwMakeContextCurrent(glfw_window_);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    return false;
  }
#endif

  glfwSetKeyCallback(glfw_window_, KeyboardCallback);

  return true;
}

void WindowImplGLFW::DestroyWindow() {
  window_surface_.reset();

  if (glfw_window_ != nullptr) {
    glfwDestroyWindow(glfw_window_);
  }
}

void WindowImplGLFW::Tick() {
  glfwPollEvents();
}
   
void WindowImplGLFW::SwapBuffers() {
  glfwSwapBuffers(glfw_window_);
}

bool WindowImplGLFW::CreateWindowSurface(const WindowSurface::CreateInfo& create_info) {
  try {
    window_surface_ = std::make_unique<WindowSurface>(create_info, glfw_window_);
  } catch (WindowSurface::InitException& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

WindowSurface* WindowImplGLFW::GetWindowSurface() {
  return window_surface_.get();
}

std::optional<event::Event> WindowImplGLFW::ConsumeEvent() {
  return event_store_.ConsumeEvent(glfw_window_);
}

bool WindowImplGLFW::ShouldClose() {
  return glfwWindowShouldClose(glfw_window_);
}

EventStore WindowImplGLFW::event_store_;

void WindowImplGLFW::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  event_store_.KeyboardCallback(window, key, scancode, action, mods);
}

std::unique_ptr<WindowImpl> WindowImpl::Create() {
  return std::make_unique<WindowImplGLFW>();
}

} // namespace
} // namespace