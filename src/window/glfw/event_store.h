#ifndef WINDOW_GLFW_EVENT_STORE_H_
#define WINDOW_GLFW_EVENT_STORE_H_

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <optional>
#include <deque>
#include "window/event.h"

namespace window {
namespace internal {

namespace {

// For each keyboard key, maps the GLFW code to the KeyInput code
const std::unordered_map<int, KeyInput> glfw_key_mapping = {
  {GLFW_KEY_A, KeyInput::KeyA}
};

} // namespace

// Shared by all windows - so that GLFW can call non-member callbacks on input
class EventStore {
public:
  std::optional<Event> ConsumeEvent(GLFWwindow* window);

  void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  std::unordered_map<GLFWwindow*, std::deque<Event>> window_events_;
};

} // namespace
} // namespace

#endif // WINDOW_GLFW_EVENT_STORE_H_