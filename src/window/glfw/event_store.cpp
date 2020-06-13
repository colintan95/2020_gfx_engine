#include "window/glfw/event_store.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

namespace window {
namespace internal {

std::optional<Event> EventStore::ConsumeEvent(GLFWwindow* window) {
  auto it = window_events_.find(window);
  if (it == window_events_.end()) {
    return std::nullopt;
  }
  std::deque<Event>& events = it->second;

  if (events.empty()) {
    return std::nullopt;
  } else {
    Event result = events.front();
    events.pop_front();
    return std::move(result);
  }
}

void EventStore::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto key_it = glfw_key_mapping.find(key);
  if (key_it == glfw_key_mapping.end()) {
    return;
  }
  KeyInput internal_key = key_it->second;

  KeyAction internal_action;
  switch (action) {
  case GLFW_PRESS:
    internal_action = KeyAction::Down;
    break;
  case GLFW_RELEASE:
    internal_action = KeyAction::Up;
    break;
  default:
    return;
  }

  KeyboardEvent event;
  event.key = internal_key;
  event.action = internal_action;
  event.modifier_key = KeyInput::None;
  window_events_[window].push_back(Event::Create(std::move(event)));
}

} // namespace
} // namespace