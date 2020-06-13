#include "gal/object.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include "gal/platform.h"

namespace gal {

namespace {

std::shared_ptr<GALId> GenerateGALId() {
  static int counter = 0;

  ++counter;
  return std::make_shared<GALId>(counter);
}

} // namespace

GALObject::GALObject(GALPlatform* platform) {
  gal_id_ = GenerateGALId();
  gal_platform_ = platform;
  platform_details_ = platform->GetPlatformDetails();

}

} // namespace