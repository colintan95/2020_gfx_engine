#ifndef GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_
#define GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/gal_texture_impl.h"

namespace gal {

class GALTextureImplGL : public IGALTextureImpl {
public:
  virtual bool Create(TextureType type, TextureFormat format, uint16_t width, uint16_t height, 
                      uint8_t* data);

  GLuint GetGLId() const { return gl_tex_id_; } 

private:
  GLuint gl_tex_id_;      
}; 

using GALTexture = GALTextureWrapper<GALTextureImplGL>;
  
} // namespace

#endif // GAL_OPENGL_GAL_TEXTURE_IMPL_GL_H_