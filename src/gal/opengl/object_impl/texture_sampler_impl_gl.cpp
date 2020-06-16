#include "gal/opengl/object_impl/texture_sampler_impl_gl.h"

namespace gal {

bool GALTextureSamplerImplGL::Create(GALPlatform* gal_platform, const GALTexture& texture) {
// TODO(colintan): Replace this with something better
  static int unit_counter = 0;

  // TODO(colintan): Check that GALTexture returns a valid GLuint
  GLuint gl_tex = texture.GetImpl().GetGLId();

  ++unit_counter;
  tex_unit_ = unit_counter;
  
  glActiveTexture(GL_TEXTURE0 + tex_unit_);
  if (texture.GetType() == TextureType::Texture2D) {
    glBindTexture(GL_TEXTURE_2D, gl_tex);
  } else {
    return false;
  }

  return true;
}

void GALTextureSamplerImplGL::Destroy() {
  
}

} // namespace