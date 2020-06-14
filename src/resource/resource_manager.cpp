#include "resource_manager.h"

namespace resource {

void ResourceManagerBase::SetupLookupTables(HandleId handle_id, ResourceBase* resource) {
  handle_to_resource_[handle_id] = resource;
  resource_to_handle_[resource] = handle_id;
}

} // namespace