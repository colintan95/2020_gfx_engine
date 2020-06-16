#ifndef GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_
#define GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/gal_buffer.h"

namespace gal {

class GALBufferImplGL : public IGALBufferImpl {
public:
  bool Create(BufferType type, uint8_t* data, size_t size);

private:
  GLuint gl_buf_id_;
}; 

using GALBufferNew = GALBufferWrapper<GALBufferImplGL>;

} // namespace

#endif // GAL_OPENGL_GAL_BUFFER_IMPL_GL_H_