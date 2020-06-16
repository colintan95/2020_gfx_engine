#ifndef GAL_OPENGL_GAL_SHADER_IMPL_GL_H_
#define GAL_OPENGL_GAL_SHADER_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/gal_shader_impl.h"

namespace gal {

class GALShaderImplGL : public IGALShaderImpl {
public:
  bool Create(ShaderType type, const std::string& source);

  GLuint GetGLId() const { return gl_shader_id_; }

private:
  GLuint gl_shader_id_;
};

using GALShader = GALShaderWrapper<GALShaderImplGL>;
  
} // namespace

#endif // GAL_OPENGL_GAL_SHADER_IMPL_GL_H_