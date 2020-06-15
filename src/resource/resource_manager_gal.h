#ifndef RESOURCE_RESOURCE_MANAGER_GAL_H_
#define RESOURCE_RESOURCE_MANAGER_GAL_H_

#include <memory>
#include <vector>
#include "resource/resource_gal.h"
#include "resource/resource_manager.h"

namespace gal {
class GALPlatform;
} // namespace

namespace resource {

class HandleGAL;

class ResourceManagerGAL : public ResourceManagerBase {
public:
  ResourceManagerGAL(gal::GALPlatform* gal_platform);
  ~ResourceManagerGAL();

  HandleGAL CreateResource(ResourceGAL::Type type, ResourceGAL::ConfigVariant config);

private:
  gal::GALPlatform* gal_platform_;

  std::vector<std::unique_ptr<ResourceGAL>> resources_;
};

} // namespace

#endif // RESOURCE_RESOURCE_MANAGER_GAL_H_