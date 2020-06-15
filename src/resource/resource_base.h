#ifndef RESOURCE_RESOURCE_BASE_H_
#define RESOURCE_RESOURCE_BASE_H_

#include <cstdint>

namespace resource {

class ResourceManagerBase;

using HandleId = uint32_t;

// TODO(colintan): Add move constructor, then call it from the base classes
class ResourceBase {
public:
  virtual ~ResourceBase() {}

  virtual void Alloc() = 0;
  virtual void Dealloc() = 0;
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
  ResourceManagerBase* manager_ = nullptr;
};

} // namespace

#endif // RESOURCE_RESOURCE_BASE_H_