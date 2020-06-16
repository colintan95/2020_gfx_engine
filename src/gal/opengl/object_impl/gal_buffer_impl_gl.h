#ifndef GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_
#define GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/object_impl/gal_buffer_impl.h"

namespace gal {

namespace internal {

class GALBufferImplGL : public IGALBufferImpl {
public:
  bool Create(GALPlatform* gal_platform, BufferType type, uint8_t* data, size_t size);
  void Destroy();

  GLuint GetGLId() const { return gl_buf_id_; }

private:
  GLuint gl_buf_id_;
}; 

} // namespace

using GALBuffer = internal::GALBuffer<internal::GALBufferImplGL>;

} // namespace

#endif // GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_