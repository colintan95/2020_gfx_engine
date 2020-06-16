#ifndef GAL_GAL_SHADER_IMPL_H_
#define GAL_GAL_SHADER_IMPL_H_

#include <string>
#include "gal/gal_object.h"

namespace gal {

enum class ShaderType {
  Invalid,
  Vertex,
  Fragment
};

namespace internal {

class IGALShaderImpl {
public:
  virtual ~IGALShaderImpl() {}

  virtual bool Create(ShaderType type, const std::string& source) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALShaderWrapper : public GALObjectBase {
public:
  GALShaderWrapper() {}

  bool Create(ShaderType type, const std::string& source) {
    if (impl_.Create(type, source)) {
      SetValid(true);
      type_ = type;
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

  ShaderType GetType() const { return type_; }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  ShaderType type_;

  ImplType impl_;
};

} // namespace
   
} // namespace

#endif // GAL_GAL_SHADER_IMPL_H_