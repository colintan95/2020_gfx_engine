#ifndef GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_
#define GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_

#include <GL/glew.h>

#include "gal/object_impl/gal_pipeline_impl.h"

namespace gal {

class GALPipelineImplGL : public IGALPipelineImpl {
public:
  bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
              const GALShader& frag_shader) final;
  void Destroy() final;

  GLuint GetProgramGLId() const { return gl_program_id_; }
  GLuint GetVaoGLId() const { return gl_vao_id_; }

private:
  GLuint gl_program_id_;
  GLuint gl_vao_id_;
};

using GALPipeline = GALPipelineBase<GALPipelineImplGL>;

} // namespace

#endif // GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_