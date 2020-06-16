#include "gal/opengl/object_impl/gal_texture_impl_gl.h"

#include <GL/glew.h>

#include <cassert>
#include "gal/platform.h"

namespace gal {
namespace internal {

bool GALTextureImplGL::Create(GALPlatform* gal_platform, TextureType type, TextureFormat format, 
                              uint16_t width, uint16_t height, uint8_t* data) {
  assert(gal_platform != nullptr);
  
  glGenTextures(1, &gl_tex_id_);

  if (type == TextureType::Texture2D && format == TextureFormat::RGB) {
    glBindTexture(GL_TEXTURE_2D, gl_tex_id_);

    // TODO(colintan): Expose these as parameters to user
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  } else {
    return false;
  }

  return true;
}

void GALTextureImplGL::Destroy() {
  glDeleteTextures(1, &gl_tex_id_);
}

} // namespace
} // namespace