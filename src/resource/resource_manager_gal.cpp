#include "resource/resource_manager_gal.h"

#include <utility>
#include <memory>
#include <variant>

namespace resource {

ResourceManagerGAL::ResourceManagerGAL(gal::GALPlatform* gal_platform) 
  : gal_platform_(gal_platform) {

}

ResourceManagerGAL::~ResourceManagerGAL() {

}

} // namespace