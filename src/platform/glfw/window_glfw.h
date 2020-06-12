#ifndef PLATFORM_GLFW_WINDOW_GLFW_H_
#define PLATFORM_GLFW_WINDOW_GLFW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include "platform/window.h"

namespace platform {
namespace internal {
class SharedState;
} // namespace
} // namespace

namespace window {

class WindowImplGLFW : public WindowImpl {
public:
  WindowImplGLFW(std::shared_ptr<platform::internal::SharedState> shared_state);

  bool Initialize() final;
  void Cleanup() final;

  bool CreateWindow(int width, int height, const std::string& title) final;

  void Tick() final;

  bool ShouldClose() final;

private:
  std::shared_ptr<platform::internal::SharedState> shared_state_;

  bool has_window_ = false;
  GLFWwindow* glfw_window_ = nullptr;

  int width_ = 0;
  int height_ = 0;
  std::string title_;
};

} // namespace

#endif // PLATFORM_GLFW_WINDOW_GLFW_H_