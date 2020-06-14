#ifndef RESOURCE_RESOURCE_MANAGER_H_
#define RESOURCE_RESOURCE_MANAGER_H_

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include "resource/resource.h"

namespace resource {

class ResourceManagerBase {
friend class HandleBase;

public: 
  // TODO(colintan): Make private
  void Invalidate(HandleId id) {
    handle_locations_[id]->Invalidate();
  }

  template<typename Resource>
  Resource* GetResource(HandleId id) {
    return dynamic_cast<Resource*>(handle_to_resource_[id]);
  }

protected:
  void SetupLookupTables(HandleId handle_id, ResourceBase* resource);

  void SetHandleLocation(HandleId id, HandleBase* handle) {
    handle_locations_[id] = handle;
  }

private:
  std::unordered_map<HandleId, ResourceBase*> handle_to_resource_;
  std::unordered_map<ResourceBase*, HandleId> resource_to_handle_;
  std::unordered_map<HandleId, HandleBase*> handle_locations_;
};

class ResourceManager : public ResourceManagerBase {
template<typename T> friend class Handle;

public:
  template<typename T>
  Handle<T> CreateResource() {
    static int counter = 0;
    ++counter;
    HandleId handle_id = counter;

    std::unique_ptr<Resource<T>> resource = std::make_unique<Resource<T>>();
    resource->Alloc();
    
    SetupLookupTables(handle_id, resource.get());
    resources_.push_back(std::move(resource));

    Handle<T> handle;
    handle.Initialize(handle_id, this);
    return std::move(handle);
  }

private:
  std::vector<std::unique_ptr<ResourceBase>> resources_;
};

} // namespace

#endif // RESOURCE_RESOURCE_MANAGER_H_