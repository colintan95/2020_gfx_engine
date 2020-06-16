#ifndef GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_

#include "gal/gal_shader.h"
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

class IGALPipelineImpl {
public:
  virtual ~IGALPipelineImpl() {}
  
  virtual bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
                      const GALShader& frag_shader) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALPipelineBase : public GALObjectBase {
public:
  bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
              const GALShader& frag_shader) {
    if (impl_.Create(gal_platform, vert_shader, frag_shader)) {
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

#endif //GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_