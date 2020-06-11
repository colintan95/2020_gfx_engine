#include "objects.h"

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

}//

GALObject::GALObject() {
  gal_id_ = GenerateGALId();
}

} // namespace