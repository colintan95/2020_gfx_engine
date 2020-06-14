#ifndef WINDOW_EVENT_CONSUMER_H_
#define WINDOW_EVENT_CONSUMER_H_

#include <deque>
#include "event/event.h"

namespace window {

class Window;

class EventConsumer {
friend class Window;

public:
  EventConsumer(Window* window);
  ~EventConsumer();

private:
  void EnqueueEvent(const event::Event& event);

private:
  Window* window_;
  std::deque<event::Event> events_;
};

} // namespace

#endif // WINDOW_EVENT_CONSUMER_H_