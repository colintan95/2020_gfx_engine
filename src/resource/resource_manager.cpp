#include "resource_manager.h"

#include <cassert>
#include <unordered_set>

namespace resource {

void ResourceManagerBase::InsertHandle(HandleId handle_id, ResourceBase* resource) {
  handle_to_resource_[handle_id] = resource;
  resource_to_handles_[resource].insert(handle_id);
}

void ResourceManagerBase::RemoveHandle(HandleId handle_id) {
  int num_erased = 0;
 
  // Remove from handle_locations_

  num_erased = handle_locations_.erase(handle_id);
  assert(num_erased == 1);
  
  // Remove from resource_to_handles_;
  
  auto resource_it = handle_to_resource_.find(handle_id);
  assert(resource_it != handle_to_resource_.end());

  if (resource_it != handle_to_resource_.end()) {
    ResourceBase* resource = resource_it->second;
    auto handles_it = resource_to_handles_.find(resource);

    assert(handles_it != resource_to_handles_.end());

    if (handles_it != resource_to_handles_.end()) {
      num_erased = handles_it->second.erase(handle_id);
      assert(num_erased == 1);
    }
  }
  
  // Remove from handle_to_resource_

  num_erased = handle_to_resource_.erase(handle_id);
  assert(num_erased == 1);
}

} // namespace