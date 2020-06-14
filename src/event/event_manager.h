#ifndef EVENT_EVENT_MANAGER_H_
#define EVENT_EVENT_MANAGER_H_

#include <memory>
#include <vector>
#include "event/event_handler.h"

namespace window {
class WindowRef;
} // namespace

namespace event {

class EventManager {
friend class HandlerRef;

public:
  EventManager(window::WindowRef* window);
  ~EventManager();

  // T should be of type IEventHandler
  template<typename T>
  std::unique_ptr<HandlerRef> CreateHandler() {
    std::unique_ptr<HandlerRef> handler_ref = std::make_unique<HandlerRef>(this);
    handler_ref->handler_ = std::make_unique<T>();
    return handler_ref;
  }

  void SendEvent(const Event& event);

private:
  void AddEventHandler(HandlerRef* handler);
  void RemoveEventHandler(HandlerRef* handler);

private:
  window::WindowRef* window_;
  std::vector<HandlerRef*> handler_refs_;
};

} // namespace

#endif // EVENT_EVENT_MANAGER_H_