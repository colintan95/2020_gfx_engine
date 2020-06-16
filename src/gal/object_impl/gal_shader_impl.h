#ifndef GAL_GAL_SHADER_IMPL_H_
#define GAL_GAL_SHADER_IMPL_H_

#include <string>
#include "gal/gal_object.h"

// TODO(colintan): Consider wrapping these classes in an internal namespace

namespace gal {

enum class ShaderType {
  Invalid,
  Vertex,
  Fragment
};

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
  GALShaderWrapper(ShaderType type, const std::string& source)  {
    if (impl_.Create(type, source)) {
      SetValid(true);
      type_ = type;
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

#endif // GAL_GAL_SHADER_IMPL_H_