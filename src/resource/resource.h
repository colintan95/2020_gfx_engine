#ifndef RESOURCE_RESOURCE_H_
#define RESOURCE_RESOURCE_H_

#include <iostream>
#include <memory>
#include <utility>
#include "resource/resource_base.h"
#include "resource/resource_manager_base.h"

namespace resource {

template<typename T>
class Resource : public ResourceBase {
public:
  Resource() {}
  ~Resource() {}

  Resource(const Resource&) = delete;
  Resource& operator=(const Resource&) = delete;
  
  Resource(Resource&& other) 
    : resource_(std::move(other.resource_)) {}
  Resource& operator=(const Resource&& other) {
    resource_ = std::move(other.resource_);
    return *this;
  }

  void Alloc() final {
    resource_ = std::make_unique<T>();
  }

  void Dealloc() final {
    resource_.reset();
  }

  const T& Get() const { return *resource_; }
  T& Get() { return *resource_; }

private:
  std::unique_ptr<T> resource_;
};

template<typename T>
class Handle : public HandleBase {
friend class ResourceManager;

public:
  Handle() {}
  Handle(ResourceManagerBase* manager, ResourceBase* resource) : HandleBase(manager, resource) {}
  ~Handle() {}

  Handle(const Handle&) = delete;
  Handle& operator=(const Handle&) = delete;

  Handle(Handle&& other) : HandleBase(std::move(other)) {}
  Handle& operator=(Handle&& rhs) { 
    HandleBase::HandleBase(std::move(other)); 
    return *this;
  }

  const T& Get() const {
    return GetInternal();
  }
  T& Get() {
    return GetInternal();
  }

private:
  T& GetInternal() {
    Resource<T>* resource = GetManager()->GetResource<Resource<T>>(GetId());
    return resource->Get();
  }
};

} // namespace

#endif // RESOURCE_RESOURCE_H_