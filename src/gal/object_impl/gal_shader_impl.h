#ifndef GAL_OBJECT_IMPL_GAL_SHADER_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_SHADER_IMPL_H_

#include <string>
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

enum class ShaderType {
  Invalid,
  Vertex,
  Fragment
};

class IGALShaderImpl {
public:
  virtual ~IGALShaderImpl() {}

  virtual bool Create(GALPlatform* gal_platform, ShaderType type, const std::string& source) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALShaderBase : public GALObjectBase {
public:
  bool Create(GALPlatform* gal_platform, ShaderType type, const std::string& source) {
    if (impl_.Create(gal_platform, type, source)) {
      SetValid(true);
      type_ = type;
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

public:
  ShaderType GetType() const { return type_; }

private:
  ShaderType type_;

  ImplType impl_;
};

} // namespace

#endif // GAL_OBJECT_IMPL_GAL_SHADER_IMPL_H_