#ifndef GAL_OBJECT_IMPL_GAL_BUFFER_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_BUFFER_IMPL_H_

#include <cstdint>
#include <exception>
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

enum class BufferType {
  Vertex,
  Uniform
};

class IGALBufferImpl {
public:
  virtual ~IGALBufferImpl() {}

  virtual void Destroy() = 0;
};

template<typename ImplType, typename BuilderType>
class GALBufferBase : public GALObjectBase {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize GALBuffer";
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
  
} // namespace gal

#endif // GAL_OBJECT_IMPL_GAL_BUFFER_IMPL_H_