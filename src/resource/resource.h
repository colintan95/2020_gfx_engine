#ifndef RESOURCE_RESOURCE_H_
#define RESOURCE_RESOURCE_H_

#include <iostream>
#include <memory>
#include <utility>

namespace resource {

class ResourceManagerBase;
class ResourceManager;

using HandleId = uint32_t;

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
  Resource(Resource&&) = delete;
  Resource& operator=(const Resource&&) = delete;

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

// template<typename T>
// class ResourceGAL : public ResourceBase {
// public:

// }

class HandleBase {

public:
  HandleBase() {}
  virtual ~HandleBase() {}

  HandleBase(const HandleBase&) = delete;
  HandleBase& operator=(const HandleBase&) = delete;

  HandleBase(HandleBase&& other);
  HandleBase& operator=(HandleBase&& other);

  void Initialize(HandleId id, ResourceManagerBase* manager) {
    valid_ = true;
    id_ = id;
    manager_ = manager;
  }

  void Invalidate() { valid_ = false; }
  
  bool IsValid() { return valid_; }

  HandleId GetId() { return id_; }

  ResourceManagerBase* GetManager() {
    return manager_;
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