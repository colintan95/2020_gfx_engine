#ifndef GAL_OPENGL_OBJECT_IMPL_GAL_TEXTURE_SAMPLER_IMPL_GL_H_
#define GAL_OPENGL_OBJECT_IMPL_GAL_TEXTURE_SAMPLER_IMPL_GL_H_

#include "gal/object_impl/gal_texture_sampler_impl.h"

namespace gal {

class GALTextureSamplerImplGL : IGALTextureSamplerImpl {
public:
  bool Create(GALPlatform* gal_platform, const GALTexture& texture) final;
  void Destroy() final;

  int GetTexUnit() const { return tex_unit_; }

private:
  int tex_unit_;
};

using GALTextureSampler = GALTextureSamplerBase<GALTextureSamplerImplGL>;

} // namespace

#endif // GAL_OPENGL_OBJECT_IMPL_GAL_TEXTURE_SAMPLER_IMPL_GL_H_