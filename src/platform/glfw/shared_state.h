#ifndef PLATFORM_GLFW_SHARED_STATE_H_
#define PLATFORM_GLFW_SHARED_STATE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

namespace platform {
namespace internal {

class SharedState{
public:
  bool CreateGLFWWindow(int width, int height, const std::string& title);

  GLFWwindow* GetGLFWWindow() { return glfw_window_; }

private:
  GLFWwindow* glfw_window_ = nullptr;
  bool has_glfw_window_ = false;
};

} // namespace
} // namespace

#endif // PLATFORM_GLFW_SHARED_STATE_H_