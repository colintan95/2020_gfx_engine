#include "gal/opengl/id_converter.h"

#include <unordered_map>

namespace gal {
namespace internal {

void IdConverter::AddGALId(GALId gal_id, GLuint gl_id) {
  gal_to_gl_[gal_id] = gl_id;
}

void IdConverter::RemoveGALId(GALId gal_id) {
  // TODO(colintan): Do we need to check if the map contains the gal id?
  gal_to_gl_.erase(gal_id);
}

std::optional<GLuint> IdConverter::ConvertGALId(GALId gal_id) {
  auto result_it = gal_to_gl_.find(gal_id);
  if (result_it == gal_to_gl_.end()) {
    return std::nullopt;
  } else {
    return result_it->second;
  }
}

} // namespace
} // namespace