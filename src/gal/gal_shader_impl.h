#ifndef GAL_GAL_SHADER_IMPL_H_
#define GAL_GAL_SHADER_IMPL_H_

#include <string>

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
class GALShaderWrapper {
public:
  GALShaderWrapper() {}
  GALShaderWrapper(ShaderType type, const std::string& source)  {
    if (impl_.Create(type, source)) {
      valid_ = true;
      type_ = type;
    }
  }

  void Destroy() {
    if (valid_) {
      impl_.Destroy();
    }
  }

  ShaderType GetType() const { return type_; }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  bool valid_ = false;
  ShaderType type_;

  ImplType impl_;
};
   
} // namespace

#endif // GAL_GAL_SHADER_IMPL_H_