#include "window/event_consumer.h"

#include <cassert>
#include <iostream>
#include <utility>
#include "window/window.h"

namespace window {

EventConsumer::EventConsumer(Window* window) {
  assert(window != nullptr);
  window_ = window;
  window_->AddEventConsumer(this);
}

EventConsumer::~EventConsumer() {
  assert(window_ != nullptr);
  window_->RemoveEventConsumer(this);
  window_ = nullptr;
}

std::optional<event::Event> EventConsumer::ConsumeEvent() {
  if (events_.empty()) {
    return std::nullopt;
  }

  // TODO(colintan): Is this copied or moved?
  event::Event event = events_.front();
  events_.pop_front();
  return std::move(event);
}

void EventConsumer::EnqueueEvent(const event::Event& event) {
  events_.push_back(event);
}

} // namespace