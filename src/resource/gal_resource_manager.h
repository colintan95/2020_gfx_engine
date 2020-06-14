#ifndef RESOURCE_GAL_RESOURCE_MANAGER_H_
#define RESOURCE_GAL_RESOURCE_MANAGER_H_

#include <memory>
#include <vector>
#include "resource/gal_resource.h"
#include "resource/resource_manager.h"

namespace gal {
class GALPlatform;
} // namespace

namespace resource {

class GALHandle;

class GALResourceManager : public ResourceManagerBase {
public:
  GALResourceManager(gal::GALPlatform* gal_platform);
  ~GALResourceManager();

  GALHandle CreateResource(GALResource::Type type, GALResource::ConfigVariant config);

private:
  gal::GALPlatform* gal_platform_;

  std::vector<std::unique_ptr<GALResource>> resources_;
};

} // namespace

#endif // RESOURCE_GAL_RESOURCE_MANAGER_H_