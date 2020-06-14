#include "resource/gal_resource.h"

#include <utility>

namespace resource {

GALResource::GALResource(Type type) : type_(type) {

}

GALResource::~GALResource() {

}

GALResource::GALResource(GALResource&& other)
  : type_(std::move(other.type_)),
    resource_(std::move(other.resource_)) {}

GALResource& GALResource::operator=(GALResource&& other) {
  type_ = std::move(other.type_);
  resource_ = std::move(other.resource_);
  return *this;
}

void GALResource::Alloc() {

}

void GALResource::Dealloc() {
  
}

GALHandle::GALHandle() {

}

GALHandle::GALHandle(ResourceManagerBase* manager, ResourceBase* base) 
  : HandleBase(manager, base) {
  resource_ = dynamic_cast<GALResource*>(base);
}

GALHandle::~GALHandle() {

}

GALHandle::GALHandle(GALHandle&& other) 
  : HandleBase(std::move(other)), resource_(std::exchange(other.resource_, nullptr)) {}

GALHandle& GALHandle::operator=(GALHandle&& other) {
  HandleBase(std::move(other));
  resource_ = std::exchange(other.resource_, nullptr);
  return *this;
}

} // namespace