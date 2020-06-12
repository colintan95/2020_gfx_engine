#include "platform/glfw/window_glfw.h"

#include <GL/glew.h>

#include <memory>
#include "platform/window.h"
#include "platform/glfw/shared_state.h"

namespace window {

WindowImplGLFW::WindowImplGLFW(std::shared_ptr<platform::internal::SharedState> shared_state) {
  shared_state_ = shared_state;
}

bool WindowImplGLFW::Initialize() {
  // glfwSetErrorCallback([](int err_code, const char* desc) {
  //   std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  // });
  return true;
}

void WindowImplGLFW::Cleanup() {
  if (has_window_) {
    glfwDestroyWindow(shared_state_->GetGLFWWindow());
  }
}

bool WindowImplGLFW::CreateWindow(int width, int height, const std::string& title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!shared_state_->CreateGLFWWindow(width, height, title)) {
    return false;
  }
  width_ = width;
  height_ = height;
  title_ = title;

  glfwMakeContextCurrent(shared_state_->GetGLFWWindow());

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
  glfwSwapBuffers(shared_state_->GetGLFWWindow());
}

bool WindowImplGLFW::ShouldClose() {
  return glfwWindowShouldClose(shared_state_->GetGLFWWindow());
}

} // namespace