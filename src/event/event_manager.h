#ifndef EVENT_EVENT_MANAGER_H_
#define EVENT_EVENT_MANAGER_H_

#include <exception>
#include <memory>
#include <vector>
#include "event/event_handler.h"
#include "window/event_consumer.h"

namespace window {
class Window;
class EventConsumer;
} // namespace

namespace event {

class EventManager {
friend class EventHandler;

public:
  class InitException : std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize EventManger.";
    }
  };

public:
  EventManager(window::Window* window);
  ~EventManager();

  // T should be of type IEventHandler
  template<typename T>
  std::unique_ptr<EventHandler> CreateHandler() {
    std::unique_ptr<EventHandler> handler = std::make_unique<EventHandler>(this);
    handler->impl_ = std::make_unique<T>();
    return handler;
  }

  void Tick();

private:
  void SendEvent(const Event& event);

  void AddEventHandler(EventHandler* handler);
  void RemoveEventHandler(EventHandler* handler);

private:
  window::Window* window_;
  std::unique_ptr<window::EventConsumer> event_consumer_;

  std::vector<EventHandler*> handlers_;
};

} // namespace

#endif // EVENT_EVENT_MANAGER_H_