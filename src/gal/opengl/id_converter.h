#ifndef GAL_OPENGL_ID_CONVERTER_H_
#define GAL_OPENGL_ID_CONVERTER_H_

#include <GL/glew.h>

#include <optional>
#include "gal/objects.h"

namespace gal {
namespace opengl {

void AddGALId(GALId gal_id, GLuint gl_id);

std::optional<GLuint> ConvertGALId(GALId gal_id);

} // namespace
} // namespace

#endif // GAL_OPENGL_ID_CONVERTER_H_