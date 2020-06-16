#include "gal/opengl/object_impl/gal_buffer_impl_gl.h"

#include <GL/glew.h>

#include <cassert>
#include "gal/gal_platform.h"

namespace gal {
namespace internal {

bool GALBufferImplGL::Create(GALPlatform* gal_platform, BufferType type, uint8_t* data, 
                             size_t size) {
  assert(gal_platform != nullptr);
  
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

void GALBufferImplGL::Destroy() {
  glDeleteBuffers(1, &gl_buf_id_);
}

} // namespace
} // namespace