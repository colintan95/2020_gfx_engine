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
};

template<typename ImplType>
class GALShaderWrapper {
public:
  GALShaderWrapper() {}
  GALShaderWrapper(ShaderType type, const std::string& source) : type_(type) {
    impl_.Create(type, source);
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