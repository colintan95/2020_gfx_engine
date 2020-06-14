#include "event/event_handler.h"

#include <cassert>
#include "event/event_manager.h"

namespace event {

EventHandler::EventHandler(EventManager* manager) {
  assert(manager != nullptr);

  manager_ = manager;
  manager_->AddEventHandler(this);
}

EventHandler::~EventHandler() {
  assert(manager_ != nullptr);
  assert(impl_ != nullptr);
  
  impl_.release();
  manager_->RemoveEventHandler(this);
  manager_ = nullptr;
}

} // namespace