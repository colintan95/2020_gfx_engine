#include "resource/image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace resource {

std::shared_ptr<Image> ImageLoader::LoadImage(const std::string& path) {
  std::shared_ptr<Image> image = std::make_shared<Image>();
  if (!LoadImage(path, *image)) {
    return nullptr;
  }
  return image;
}

bool ImageLoader::LoadImage(const std::string& path, Image& out_image) {
  // TODO(colintan): Make sure that |out_image| is set back to default state first

  stbi_set_flip_vertically_on_load(true);

  int load_width = -1;
  int load_height = -1;
  int load_channels = -1;
  stbi_uc *stb_pixels = stbi_load(path.c_str(), &load_width, &load_height, 
                              &load_channels, 0);
  if (stb_pixels == nullptr) {
    return false;
  }                 

  out_image.width = static_cast<uint32_t>(load_width);
  out_image.height  = static_cast<uint32_t>(load_height);

  switch (load_channels) {
    case 4:
      out_image.format = ImageFormat::RGBA;
      break;
    case 3:
      out_image.format = ImageFormat::RGB;
      break;
    default:
      out_image.format = ImageFormat::Invalid;
  }

  // TODO(colintan): Do checked arithmetic
  uint32_t img_size = out_image.width * out_image.height * static_cast<uint32_t>(load_channels);
  out_image.pixels = std::vector<uint8_t>(stb_pixels, stb_pixels + img_size);

  return true;
}

} // namespace