#include "event/event_manager.h"

#include <algorithm>
#include <memory>
#include "window/window.h"

namespace event {

bool EventManager::Initialize(window::Window* window) {
  window_ = window;
  return true;
}

void EventManager::Cleanup() {
  window_ = nullptr;
}

void EventManager::SendEvent(const Event& event) {
  for (HandlerRef* handler_ref : handler_refs_) {
    handler_ref->handler_->Handle(event);
  }
}

void EventManager::AddEventHandler(HandlerRef* handler_ref) {
  handler_refs_.push_back(handler_ref);
}

void EventManager::RemoveEventHandler(HandlerRef* handler_ref) {
  auto it = std::find(handler_refs_.begin(), handler_refs_.end(), handler_ref);
  if (it != handler_refs_.end()) {
    handler_refs_.erase(it);
  }
}

} // namespace