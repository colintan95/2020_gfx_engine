#ifndef GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_

#include <cstdint>
#include <exception>
#include <utility>
#include <vector>
#include "gal/gal_shader.h"
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

class IGALPipelineImpl {
public:
  virtual ~IGALPipelineImpl() {}
  
  virtual void Destroy() = 0;
};

template<typename ImplType, typename BuilderType>
class GALPipelineBase : public GALObjectBase {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize GALPipeline";
    }
  };

public:
  static BuilderType BeginBuild(GALPlatform* gal_platform) {
    return BuilderType(gal_platform);
  }

  void Destroy() {
    if (IsValid()) { impl_.Destroy(); }
  }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  ImplType impl_;
};

} // namespace

#endif //GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_