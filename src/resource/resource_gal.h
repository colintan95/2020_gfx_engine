#ifndef RESOURCE_RESOURCE_GAL_H_
#define RESOURCE_RESOURCE_GAL_H_

#include <cassert>
#include <cstdint>
#include <variant>
#include "gal/gal_buffer.h"
#include "gal/object.h"
#include "resource/resource_base.h"
#include "resource/resource_manager_base.h"

namespace gal {
class GALPlatform;
} // namespace

namespace resource {

template<typename T>
class ResourceGAL : public ResourceBase {
friend class ResourceManagerGAL;

public:
  ResourceGAL() {}
  ~ResourceGAL() {}

  ResourceGAL(const ResourceGAL&) = delete;
  ResourceGAL& operator=(const ResourceGAL&) = delete;

  ResourceGAL(ResourceGAL&& other)
    : resource_(std::move(other.resource_)) {}

  ResourceGAL& operator=(ResourceGAL&& other) {
    resource_ = std::move(other.resource_);
    return *this;
  }

  void Alloc() final {}
  void Dealloc() final {}

  const T& Get() const { return GetInternal(); }
  T& Get() { return GetInternal(); }

private:
  T& GetInternal() { return resource_; }

private:
  T resource_;
};

template<typename T>
class HandleGAL : public HandleBase {
public:
  HandleGAL() {}
  HandleGAL(ResourceManagerBase* manager, ResourceBase* base)
    : HandleBase(manager, base) {
    resource_ = dynamic_cast<ResourceGAL<T>*>(base);
  }
  ~HandleGAL() {}

  HandleGAL(const HandleGAL&) = delete;
  HandleGAL& operator=(const HandleGAL&) = delete;

  HandleGAL(HandleGAL&& other) 
    :  HandleBase(std::move(other)),
       resource_(std::exchange(other.resource_, nullptr)) {}

  HandleGAL& operator=(HandleGAL&& other) {
    HandleBase(std::move(other));
    resource_ = std::exchange(other.resource_, nullptr);
    return *this;
  }

  // Copy should be explicit - don't want accidental copies since they may be expensive
  HandleGAL Clone(const HandleGAL& other) {
    ResourceManagerBase* manager = other.GetManager();
    ResourceBase* resource = manager->GetResource(other.GetId());

    HandleGAL handle(manager, resource);
    handle.resource_ = other.resource_;
    return std::move(handle);
  }

  const T& Get() const { return GetInternal(); }
  T& Get() { return GetInternal(); }

private:
  T& GetInternal() {
    return resource_->Get();
  }

private:
  ResourceGAL<T>* resource_;
};

using HandleGALBuffer = resource::HandleGAL<gal::GALBuffer>;
using HandleGALTexture = resource::HandleGAL<gal::GALTexture>;


} // namespace

#endif // RESOURCE_RESOURCE_GAL_H_