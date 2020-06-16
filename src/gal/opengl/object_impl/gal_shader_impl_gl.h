#ifndef GAL_OPENGL_GAL_SHADER_IMPL_GL_H_
#define GAL_OPENGL_GAL_SHADER_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/object_impl/gal_shader_impl.h"

namespace gal {

namespace internal {

class GALShaderImplGL : public IGALShaderImpl {
public:
  bool Create(GALPlatform* gal_platform, ShaderType type, const std::string& source) final;
  void Destroy() final;

  GLuint GetGLId() const { return gl_shader_id_; }

private:
  GLuint gl_shader_id_;
};

} // namespace

using GALShader = internal::GALShaderWrapper<internal::GALShaderImplGL>;
  
} // namespace

#endif // GAL_OPENGL_GAL_SHADER_IMPL_GL_H_