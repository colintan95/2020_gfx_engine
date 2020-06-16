#ifndef GAL_OBJECT_IMPL_TEXTURE_SAMPLER_IMPL_H_
#define GAL_OBJECT_IMPL_TEXTURE_SAMPLER_IMPL_H_

#include "gal/gal_texture.h"
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

class IGALTextureSamplerImpl {
public:
  virtual ~IGALTextureSamplerImpl() {}

  virtual bool Create(GALPlatform* gal_platform, const GALTexture& texture) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALTextureSamplerBase : public GALObjectBase {
public:
  bool Create(GALPlatform* gal_platform, const GALTexture& texture) {
    if (impl_.Create(gal_platform, texture)) {
      SetValid(true);
      return true;
    } else {
      SetValid(false);
      return false;
    }
  }

  void Destroy() {
    if (IsValid()) {
      impl_.Destroy();
    }
  }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  ImplType impl_;
};

} // namespace

#endif // GAL_OBJECT_IMPL_TEXTURE_SAMPLER_IMPL_H_