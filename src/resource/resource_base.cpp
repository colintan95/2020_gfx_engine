#include "resource/resource_base.h"

#include <cassert>
#include <utility>
#include "resource/resource_manager_base.h"

namespace resource {

HandleBase::HandleBase() {}

HandleBase::HandleBase(ResourceManagerBase* manager, ResourceBase* resource) 
  : valid_(true),
    id_(GenerateId()),
    manager_(manager) {
  assert(manager_ != nullptr);
  manager_->InsertHandle(id_, resource);
}

HandleBase::~HandleBase() {
  if (manager_ != nullptr) {
    manager_->RemoveHandle(id_);
  }
}

HandleBase::HandleBase(HandleBase&& other) 
  : valid_(std::move(other.valid_)), 
    id_(std::move(other.id_)),
    manager_(std::exchange(other.manager_, nullptr)) {
  manager_->SetHandleLocation(id_, this);
}

HandleBase& HandleBase::operator=(HandleBase&& other) {
  valid_ = std::move(other.valid_);
  id_ = std::move(other.id_);
  manager_ = std::exchange(other.manager_, nullptr);
  manager_->SetHandleLocation(id_, this);

  return *this;
}

} // namespace