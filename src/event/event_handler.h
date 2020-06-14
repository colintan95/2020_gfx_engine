#ifndef EVENT_EVENT_HANDLER_H_
#define EVENT_EVENT_HANDLER_H_

#include <memory>
#include "event/event.h"

namespace event {

class EventManager;

// Override this
class IEventHandlerImpl {
public:
  virtual ~IEventHandlerImpl() {}

  virtual void Handle(const Event& event) = 0;
};

// TODO(colintan): Rename this
class EventHandler {
friend class EventManager;

public:
  // TODO(colintan): Try to make this private if possible
  EventHandler(EventManager* manager);
  ~EventHandler();

private:
  std::unique_ptr<IEventHandlerImpl> impl_;
  EventManager* manager_;
};

} // namespace

#endif // EVENT_EVENT_HANDLER_H_