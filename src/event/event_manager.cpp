#include "event/event_manager.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <new>
#include <optional>
#include "window/event_consumer.h"
#include "window/window.h"

namespace event {

EventManager::EventManager(window::Window* window) {
  assert(window != nullptr);
  window_ = window;

  try {
    event_consumer_ = std::make_unique<window::EventConsumer>(window);
  } catch (std::bad_alloc& ba) {
    throw InitException();
  }
}

EventManager::~EventManager() {}

void EventManager::Tick() {
  while (std::optional<event::Event> event_opt = event_consumer_->ConsumeEvent()) {
    SendEvent(*event_opt);
  }
}

void EventManager::SendEvent(const Event& event) {
  for (EventHandler* handler : handlers_) {
    handler->impl_->Handle(event);
  }
}

void EventManager::AddEventHandler(EventHandler* handler) {
  handlers_.push_back(handler);
}

void EventManager::RemoveEventHandler(EventHandler* handler) {
  auto it = std::find(handlers_.begin(), handlers_.end(), handler);
  if (it != handlers_.end()) {
    handlers_.erase(it);
  }
}

} // namespace