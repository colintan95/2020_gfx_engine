#ifndef RESOURCE_RESOURCE_MANAGER_H_
#define RESOURCE_RESOURCE_MANAGER_H_

#include <cassert>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "resource/resource.h"

namespace resource {

// TODO(colintan): Do I need custom Initialize() and Cleanup() functions?
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
  template<typename HandleType>
  HandleType CreateHandle(ResourceBase* resource) {
    HandleType handle(this, resource);
    return std::move(handle);
  }

  // TODO(colintan): Need to test this
  void InvalidateHandles(ResourceBase* resource) {
    assert(resource != nullptr);

    auto handles_it = resource_to_handles_.find(resource);

    if (handles_it != resource_to_handles_.end()) {
      for (HandleId handle_id : handles_it->second) {

        auto handle_ptr_it = handle_locations_.find(handle_id);
        
        if (handle_ptr_it != handle_locations_.end()) {
          HandleBase* handle_ptr = handle_ptr_it->second;
          handle_ptr->Invalidate();
        }
      }
    }
  }

private:
  void SetHandleLocation(HandleId id, HandleBase* handle) {
    handle_locations_[id] = handle;
  }

  void InsertHandle(HandleId id, ResourceBase* resource);
  void RemoveHandle(HandleId id);

private:
  std::unordered_map<HandleId, ResourceBase*> handle_to_resource_;
  std::unordered_map<ResourceBase*, std::unordered_set<HandleId>> resource_to_handles_;
  std::unordered_map<HandleId, HandleBase*> handle_locations_;
};

class ResourceManager : public ResourceManagerBase {
template<typename T> friend class Handle;

public:
  template<typename T>
  Handle<T> CreateResource() {
    std::unique_ptr<Resource<T>> resource = std::make_unique<Resource<T>>();
    resource->Alloc();

    Handle<T> handle = CreateHandle<Handle<T>>(resource.get());
    resources_.push_back(std::move(resource));

    return std::move(handle);
  }

private:
  std::vector<std::unique_ptr<ResourceBase>> resources_;
};

} // namespace

#endif // RESOURCE_RESOURCE_MANAGER_H_