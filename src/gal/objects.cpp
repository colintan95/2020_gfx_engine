#include "gal/objects.h"

#include <cstdint>
#include <optional>
#include <string>

namespace gal {

namespace {

GALId GenerateGALId() {
  static int counter = 0;

  ++counter;
  return static_cast<GALId>(counter);
}

} // namespace

GALObject::GALObject() {
  gal_id_ = GenerateGALId();
}

} // namespace