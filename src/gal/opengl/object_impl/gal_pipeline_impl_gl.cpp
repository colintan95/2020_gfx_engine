#include "gal/opengl/object_impl/gal_pipeline_impl_gl.h"

#include <GL/glew.h>

#include <cassert>
#include <iostream>
#include "gal/gal_platform.h"

namespace gal {
namespace internal {

namespace {

bool CheckProgramSuccess(int program) {
  int gl_success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(program, 256, nullptr, log);

    // TODO(colintan): Do better logging
    std::cerr << "Failed to link program." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

} // namespace

bool GALPipelineImplGL::Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
                               const GALShader& frag_shader) {
  assert(gal_platform != nullptr);

  gl_program_id_ = glCreateProgram();

  // TODO(colintan): Check that shaders are valid
  glAttachShader(gl_program_id_, vert_shader.GetImpl().GetGLId());
  glAttachShader(gl_program_id_, frag_shader.GetImpl().GetGLId());
  
  glLinkProgram(gl_program_id_);
  if (!CheckProgramSuccess(gl_program_id_)) {
    return false;
  }

  return true;
}

void GALPipelineImplGL::Destroy() {
  glDeleteProgram(gl_program_id_);
}

} // namespace
} // namespace