#ifndef GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_
#define GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/object_impl/gal_texture_impl.h"

namespace gal {

class GALTextureImplGL : public IGALTextureImpl {
public:
  bool Create(GALPlatform* gal_platform, TextureType type, TextureFormat format, uint16_t width, 
              uint16_t height, uint8_t* data) final;
  void Destroy() final;

  GLuint GetGLId() const { return gl_tex_id_; } 

private:
  GLuint gl_tex_id_;      
}; 

using GALTexture = GALTextureBase<GALTextureImplGL>;

} // namespace

#endif // GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_