#ifndef EVENT_EVENT_HANDLER_H_
#define EVENT_EVENT_HANDLER_H_

#include <memory>
#include "event/event.h"

namespace event {

class EventManager;

// Override this
class IEventHandler {
public:
  virtual ~IEventHandler() {}

  virtual void Handle(const Event& event) = 0;
};

class HandlerRef {
friend class EventManager;

public:
  // TODO(colintan): Try to make this private if possible
  HandlerRef(EventManager* manager);
  ~HandlerRef();

private:
  std::unique_ptr<IEventHandler> handler_;
  EventManager* manager_;
};

} // namespace

#endif // EVENT_EVENT_HANDLER_H_