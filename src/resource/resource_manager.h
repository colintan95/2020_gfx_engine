#ifndef RESOURCE_RESOURCE_MANAGER_H_
#define RESOURCE_RESOURCE_MANAGER_H_

#include <memory>
#include <utility>
#include <vector>
#include "resource/resource_base.h"
#include "resource/resource_manager_base.h"

namespace resource {

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