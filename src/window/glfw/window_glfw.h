#ifndef WINDOW_GLFW_WINDOW_GLFW_H_
#define WINDOW_GLFW_WINDOW_GLFW_H_

#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include "window/glfw/event_store.h"
#include "window/window_internal.h"

namespace window {
namespace internal {

class WindowGLFW : public Window {
public:
  bool CreateWindow(int width, int height, const std::string& title) final;
  void DestroyWindow() final;

  void Tick() final;

  void SwapBuffers() final;

  std::optional<Event> ConsumeEvent() final;

private:
  static EventStore event_store_;
  static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  GLFWwindow* glfw_window_;
  int width_;
  int height_;
  std::string title_;
};

} // namespace
} // namespace

#endif // WINDOW_GLFW_WINDOW_GLFW_H_