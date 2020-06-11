#include "id_converter.h"

#include <unordered_map>
#include "../objects.h"

namespace gal {
namespace opengl {

namespace {

std::unordered_map<GALId, GLuint> gal_to_gl;

} // namespace

void AddGALId(GALId gal_id, GLuint gl_id) {
  gal_to_gl[gal_id] = gl_id;
}

std::optional<GLuint> ConvertGALId(GALId gal_id) {
  auto result_it = gal_to_gl.find(gal_id);
  if (result_it == gal_to_gl.end()) {
    return std::nullopt;
  } else {
    return result_it->second;
  }
}

} // namespace
} // namespace