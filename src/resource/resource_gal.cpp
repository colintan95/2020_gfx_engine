#include "resource/resource_gal.h"

#include <utility>

namespace resource {

ResourceGAL::ResourceGAL(Type type) : type_(type) {

}

ResourceGAL::~ResourceGAL() {

}

ResourceGAL::ResourceGAL(ResourceGAL&& other)
  : type_(std::move(other.type_)),
    resource_(std::move(other.resource_)) {}

ResourceGAL& ResourceGAL::operator=(ResourceGAL&& other) {
  type_ = std::move(other.type_);
  resource_ = std::move(other.resource_);
  return *this;
}

void ResourceGAL::Alloc() {

}

void ResourceGAL::Dealloc() {
  
}

HandleGAL::HandleGAL() {

}

HandleGAL::HandleGAL(ResourceManagerBase* manager, ResourceBase* base) 
  : HandleBase(manager, base) {
  resource_ = dynamic_cast<ResourceGAL*>(base);
}

HandleGAL::~HandleGAL() {

}

HandleGAL::HandleGAL(HandleGAL&& other) 
  : HandleBase(std::move(other)), resource_(std::exchange(other.resource_, nullptr)) {}

HandleGAL& HandleGAL::operator=(HandleGAL&& other) {
  HandleBase(std::move(other));
  resource_ = std::exchange(other.resource_, nullptr);
  return *this;
}

} // namespace