#include "gal/object.h"

#include <GL/glew.h>

#include <iostream>
#include <utility>
#include "gal/opengl/id_converter.h"
#include "gal/gal_shader.h"
#include "gal/opengl/gal_platform_gl.h"

namespace gal {

// std::optional<GALTextureSampler> GALTextureSampler::Create(GALPlatform* platform, 
//                                                            const GALTexture& texture) {
//   // TODO(colintan): Replace this with something better
//   static GLuint unit_counter = 0;

//   // TODO(colintan): Check that GALTexture returns a valid GLuint
//   GLuint gl_tex = texture.GetImpl().GetGLId();

//   ++unit_counter;
//   GLuint gl_tex_unit = unit_counter;
//   glActiveTexture(GL_TEXTURE0 + gl_tex_unit);
//   if (texture.GetType() == TextureType::Texture2D) {
//     glBindTexture(GL_TEXTURE_2D, gl_tex);
//   } else {
//     return std::nullopt;
//   }

//   GALTextureSampler result{platform};
//   platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_tex_unit);
//   result.texture_ = texture;

//   return result;
// }

// GALTextureSampler::~GALTextureSampler() {}

} // namespace