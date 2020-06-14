#include "event/event_handler.h"

#include <cassert>
#include "event/event_manager.h"

namespace event {

HandlerRef::HandlerRef(EventManager* manager) {
  assert(manager != nullptr);

  manager_ = manager;
  manager_->AddEventHandler(this);
}

HandlerRef::~HandlerRef() {
  assert(manager_ != nullptr);
  assert(handler_ != nullptr);
  
  handler_.release();
  manager_->RemoveEventHandler(this);
  manager_ = nullptr;
}

} // namespace