#ifndef GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_
#define GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_

#include <GL/glew.h>


#include "gal/object_impl/gal_pipeline_impl.h"

namespace gal {

namespace internal {

class GALPipelineImplGL : public IGALPipelineImpl {
public:
  bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
              const GALShader& frag_shader) final;
  void Destroy() final;

  GLuint GetGLId() const { return gl_program_id_; }

private:
  GLuint gl_program_id_;
};

} // namespace

using GALPipeline = internal::GALPipelineBase<internal::GALPipelineImplGL>;

} // namespace

#endif // GAL_OPENGL_OBJECT_IMPL_GAL_PIPELINE_IMPL_GL_H_