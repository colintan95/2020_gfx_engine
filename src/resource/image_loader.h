#ifndef RESOURCE_IMAGE_LOADER_H_
#define RESOURCE_IMAGE_LOADER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace resource {

enum class ImageFormat {
  Invalid,
  RGB,
  RGBA
};

struct Image {
  uint32_t width = 0;
  uint32_t height = 0;
  ImageFormat format = ImageFormat::Invalid;
  std::vector<uint8_t> pixels;
};

class ImageLoader {
public:
  std::shared_ptr<Image> LoadImage(const std::string& path);
};

} // namespace

#endif // RESOURCE_IMAGE_LOADER_H_