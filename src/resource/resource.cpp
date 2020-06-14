#include "resource/resource.h"

#include "resource/resource_manager.h"

namespace resource {

HandleBase::HandleBase(HandleBase&& other) 
  : valid_(std::move(other.valid_)), id_(std::move(other.id_)),
    manager_(std::move(other.manager_)) {
  manager_->SetHandleLocation(id_, this);
}

HandleBase& HandleBase::operator=(HandleBase&& other) {
  valid_ = other.valid_;
  id_ = std::move(other.id_);
  manager_ = other.manager_;
  manager_->SetHandleLocation(id_, this);
  return *this;
}

} // namespace