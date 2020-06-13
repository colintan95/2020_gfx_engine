#include "window/glfw/event_store.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

namespace window {
namespace internal {

std::optional<event::Event> EventStore::ConsumeEvent(GLFWwindow* window) {
  auto it = window_events_.find(window);
  if (it == window_events_.end()) {
    return std::nullopt;
  }
  std::deque<event::Event>& events = it->second;

  if (events.empty()) {
    return std::nullopt;
  } else {
    event::Event result = events.front();
    events.pop_front();
    return std::move(result);
  }
}

void EventStore::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto key_it = glfw_key_mapping.find(key);
  if (key_it == glfw_key_mapping.end()) {
    return;
  }
  event::KeyInput internal_key = key_it->second;

  event::KeyAction internal_action;
  switch (action) {
  case GLFW_PRESS:
    internal_action = event::KeyAction::Down;
    break;
  case GLFW_RELEASE:
    internal_action = event::KeyAction::Up;
    break;
  default:
    return;
  }

  event::KeyboardEvent event;
  event.key = internal_key;
  event.action = internal_action;
  event.modifier_key = event::KeyInput::None;
  window_events_[window].push_back(event::Event::Create(std::move(event)));
}

} // namespace
} // namespace