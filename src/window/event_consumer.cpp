#include "window/event_consumer.h"

#include <cassert>
#include "window/window.h"

namespace window {

EventConsumer::EventConsumer(WindowRef* window) {
  assert(window != nullptr);
  window_ = window;
  window_->AddEventConsumer(this);
}

EventConsumer::~EventConsumer() {
  assert(window_ != nullptr);
  window_->RemoveEventConsumer(this);
  window_ = nullptr;
}

void EventConsumer::EnqueueEvent(const event::Event& event) {
  events_.push_back(event);
}

} // namespace