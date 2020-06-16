#ifndef GAL_GAL_BUFFER_IMPL_H_
#define GAL_GAL_BUFFER_IMPL_H_

#include <cstdint>
#include "gal/gal_object.h"

namespace gal {

enum class BufferType {
  Vertex,
  Uniform
};

namespace internal {

class IGALBufferImpl {
public:
  virtual ~IGALBufferImpl() {}

  virtual bool Create(BufferType type, uint8_t* data, size_t size) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALBufferWrapper : public GALObjectBase {
public:
  GALBufferWrapper() {}

  bool Create(BufferType type, uint8_t* data, size_t size) {
    if (impl_.Create(type, data, size)) {
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

  BufferType GetType() const { return type_; }
  size_t GetSize() const { return size_; }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  BufferType type_;
  size_t size_;
  
  ImplType impl_;
};

} // namespace
  
} // namespace

#endif // GAL_GAL_BUFFER_IMPL_H_