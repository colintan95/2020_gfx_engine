#ifndef GAL_GAL_BUFFER_IMPL_H_
#define GAL_GAL_BUFFER_IMPL_H_

#include <cstdint>
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

enum class BufferType {
  Vertex,
  Uniform
};

namespace internal {

class IGALBufferImpl {
public:
  virtual ~IGALBufferImpl() {}

  virtual bool Create(GALPlatform* gal_platform, BufferType type, uint8_t* data, size_t size) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALBufferBase : public GALObjectBase {
public:
  bool Create(GALPlatform* gal_platform, BufferType type, uint8_t* data, size_t size) {
    if (impl_.Create(gal_platform, type, data, size)) {
      SetValid(true);
      type_ = type;
      size_ = size;
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
  BufferType GetType() const { return type_; }
  size_t GetSize() const { return size_; }

private:
  BufferType type_;
  size_t size_;
  
  ImplType impl_;
};

} // namespace
  
} // namespace

#endif // GAL_GAL_BUFFER_IMPL_H_