#include "gal/opengl/gal_shader_impl_gl.h"

#include <GL/glew.h>

#include <iostream>

namespace gal {

namespace {

bool CheckShaderSuccess(int shader) {
  int gl_success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(shader, 256, nullptr, log);

    // TODO(colintan): Do better logging
    std::cerr << "Failed to compile shader." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

} // namespace
   
bool GALShaderImplGL::Create(ShaderType type, const std::string& source) {
  switch (type) {
  case ShaderType::Vertex:
    gl_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
    break;
  case ShaderType::Fragment:
    gl_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  default:
    return false;
  }

  const char* sources[] = { source.c_str() };
  glShaderSource(gl_shader_id_, 1, sources, nullptr);
  glCompileShader(gl_shader_id_);
  if (!CheckShaderSuccess(gl_shader_id_)) {
    std::exit(EXIT_FAILURE);
  }
  
  return true;
}

} // namespace