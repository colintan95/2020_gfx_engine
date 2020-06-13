#ifndef WINDOW_GLFW_WINDOW_INTERNAL_GLFW_H_
#define WINDOW_GLFW_WINDOW_INTERNAL_GLFW_H_

#include <GLFW/glfw3.h>

#include <string>
#include "window/window_internal.h"

namespace window {

class WindowInternalGLFW : public WindowInternal {
public:
  bool CreateWindow(int width, int height, const std::string& title) final;
  void DestroyWindow() final;

  void SwapBuffers() final;

private:
  GLFWwindow* glfw_window_;
  int width_;
  int height_;
  std::string title_;
};

} // namespace

#endif // WINDOW_GLFW_WINDOW_INTERNAL_GLFW_H_