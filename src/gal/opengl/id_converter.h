#ifndef GAL_OPENGL_ID_CONVERTER_H_
#define GAL_OPENGL_ID_CONVERTER_H_

#include <GL/glew.h>

#include <optional>
#include <unordered_map>
#include "gal/object.h"

namespace gal {
namespace internal {

class IdConverter {
public:
  void AddGALId(GALId gal_id, GLuint gl_id);
  void RemoveGALId(GALId gal_id);

  std::optional<GLuint> ConvertGALId(GALId gal_id);

private:
  std::unordered_map<GALId, GLuint> gal_to_gl_;
};

} // namespace
} // namespace

#endif // GAL_OPENGL_ID_CONVERTER_H_