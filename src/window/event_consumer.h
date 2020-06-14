#ifndef WINDOW_EVENT_CONSUMER_H_
#define WINDOW_EVENT_CONSUMER_H_

#include <deque>
#include "event/event.h"

namespace window {

class WindowRef;

class EventConsumer {
friend class WindowRef;

public:
  EventConsumer(WindowRef* window);
  ~EventConsumer();

private:
  void EnqueueEvent(const event::Event& event);

private:
  WindowRef* window_;
  std::deque<event::Event> events_;
};

} // namespace

#endif // WINDOW_EVENT_CONSUMER_H_