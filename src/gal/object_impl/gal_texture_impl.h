#ifndef GAL_GAL_TEXTURE_IMPL_H_
#define GAL_GAL_TEXTURE_IMPL_H_

#include <cstdint>
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

enum class TextureType {
  Texture2D,
  Texture3D
};

enum class TextureFormat {
  Invalid,
  RGB,
  RGBA
};

namespace internal {

class IGALTextureImpl {
public:
  virtual ~IGALTextureImpl() {}

  virtual bool Create(GALPlatform* gal_platform, TextureType type, TextureFormat format, 
                      uint16_t width, uint16_t height, uint8_t* data) = 0;
  virtual void Destroy() = 0;
};

template<typename ImplType>
class GALTextureBase : public GALObjectBase {
public:
  bool Create(GALPlatform* gal_platform, TextureType type, TextureFormat format, uint16_t width, 
              uint16_t height, uint8_t* data) {
    if (impl_.Create(gal_platform, type, format, width, height, data)) {
      SetValid(true);
      type_ = type;
      format_ = format;
      width_ = width;
      height_ = height;
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
  TextureType GetType() const { return type_; }

private:
  TextureType type_;
  TextureFormat format_;
  uint16_t width_;
  uint16_t height_;
  
  ImplType impl_;
};

} // namespace

} // namespace

#endif // GAL_GAL_TEXTURE_IMPL_H_