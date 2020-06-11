#ifndef WINDOW_WINDOW_IMPL_H_
#define WINDOW_WINDOW_IMPL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace window {

class WindowImpl {
public:
  WindowImpl(int width, int height, const std::string& title);
  ~WindowImpl();

  void Tick();

  bool ShouldClose();

private:
  bool initialized_ = false;
  GLFWwindow* glfw_window_ = nullptr;

  int width_ = 0;
  int height_ = 0;
  std::string title_;
};

} // namespace

#endif // WINDOW_WINDOW_IMPL_H_