#ifndef GAL_GAL_BUFFER_IMPL_H_
#define GAL_GAL_BUFFER_IMPL_H_

#include <cstdint>
#include "object.h"

namespace gal {
   
class IGALBufferImpl {
public:
  virtual ~IGALBufferImpl() {}

  virtual bool Create(BufferType type, uint8_t* data, size_t size) = 0;
};

template<typename ImplType>
class GALBufferWrapper {
public:
  GALBufferWrapper() {}
  GALBufferWrapper(BufferType type, uint8_t* data, size_t size)
    : type_(type), size_(size) {
    impl_.Create(type, data, size);
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

#endif // GAL_GAL_BUFFER_IMPL_H_