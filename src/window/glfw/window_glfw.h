#ifndef WINDOW_GLFW_WINDOW_GLFW_H_
#define WINDOW_GLFW_WINDOW_GLFW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

namespace window {

class WindowImpl {
public:
  bool Initialize(int width, int height, const std::string& title);
  void Destroy();

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

#endif // WINDOW_GLFW_WINDOW_GLFW_H_