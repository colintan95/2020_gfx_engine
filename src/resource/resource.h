#ifndef RESOURCE_RESOURCE_H_
#define RESOURCE_RESOURCE_H_

#include <iostream>
#include <memory>
#include <utility>

namespace resource {

class ResourceManagerBase;
class ResourceManager;

using HandleId = uint32_t;

// TODO(colintan): Add move constructor, then call it from the base classes
class ResourceBase {
public:
  virtual ~ResourceBase() {}

  virtual void Alloc() = 0;
  virtual void Dealloc() = 0;
};

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
    resource_.release();
  }

  const T& Get() const { return *resource_; }
  T& Get() { return *resource_; }

private:
  std::unique_ptr<T> resource_;
};

class HandleBase {

public:
  HandleBase();
  HandleBase(ResourceManagerBase* manager, ResourceBase* resource);
  virtual ~HandleBase();

  HandleBase(const HandleBase&) = delete;
  HandleBase& operator=(const HandleBase&) = delete;

  HandleBase(HandleBase&& other);
  HandleBase& operator=(HandleBase&& other);

  void Invalidate() { valid_ = false; }
  
  bool IsValid() { return valid_; }

  HandleId GetId() { return id_; }

  ResourceManagerBase* GetManager() {
    return manager_;
  }

private:
  HandleId GenerateId() {
    static int counter = 0;
    ++counter;
    return counter;
  }

private:
  bool valid_ = false;
  HandleId id_ = 0;
  ResourceManagerBase* manager_;
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