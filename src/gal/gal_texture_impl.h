#ifndef GAL_GAL_TEXTURE_IMPL_H_
#define GAL_GAL_TEXTURE_IMPL_H_

#include <cstdint>

namespace gal {

enum class TextureType {
  Texture2D,
  Texture3D
};

enum class TextureFormat {
  Invalid,
  RGB,
  RGBA
};

class IGALTextureImpl {
public:
  virtual ~IGALTextureImpl() {}

  virtual bool Create(TextureType type, TextureFormat format, uint16_t width, uint16_t height, 
                      uint8_t* data) = 0;
};

template<typename ImplType>
class GALTextureWrapper {
public:
  GALTextureWrapper() {}
  GALTextureWrapper(TextureType type, TextureFormat format, uint16_t width, uint16_t height, 
                    uint8_t* data)
    : type_(type), format_(format), width_(width), height_(height) {
    impl_.Create(type, format, width, height, data);
  }

  TextureType GetType() const { return type_; }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  TextureType type_;
  TextureFormat format_;
  uint16_t width_;
  uint16_t height_;
  
  ImplType impl_;
};

} // namespace

#endif // GAL_GAL_TEXTURE_IMPL_H_