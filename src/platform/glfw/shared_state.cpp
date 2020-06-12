#include "platform/glfw/shared_state.h"

namespace platform {
namespace internal {

bool SharedState::CreateGLFWWindow(int width, int height, const std::string& title) {
  glfw_window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (glfw_window_ == nullptr) {
    return false;
  }

  has_glfw_window_ = true;
  return true;
}

} // namespace
} // namespace