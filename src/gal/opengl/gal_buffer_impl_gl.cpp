#include "gal/opengl/gal_buffer_impl_gl.h"

#include <GL/glew.h>

namespace gal {

bool GALBufferImplGL::Create(BufferType type, uint8_t* data, size_t size) {
  glCreateBuffers(1, &gl_buf_id_);
  
  GLuint buf_target;
  switch (type) {
  case BufferType::Vertex:
    buf_target = GL_ARRAY_BUFFER;
    break;
  case BufferType::Uniform:
    buf_target = GL_UNIFORM_BUFFER;
    break;
  default:
    return false;
  }

  glBindBuffer(buf_target, gl_buf_id_);
  glBufferData(buf_target, size, data, GL_STATIC_DRAW);
  glBindBuffer(buf_target, 0);

  return true;
}

} // namespace