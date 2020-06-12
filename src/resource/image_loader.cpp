#include "resource/image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace resource {

std::shared_ptr<Image> ImageLoader::LoadImage(const std::string& path) {
  // TODO(colintan): Check if this is needed
  stbi_set_flip_vertically_on_load(true);

  int load_width = -1;
  int load_height = -1;
  int load_channels = -1;
  stbi_uc *stb_pixels = stbi_load(path.c_str(), &load_width, &load_height, 
                              &load_channels, 0);
  if (stb_pixels == nullptr) {
    return nullptr;
  }                 

  std::shared_ptr<Image> image = std::make_shared<Image>();

  image->width = static_cast<uint32_t>(load_width);
  image->height  = static_cast<uint32_t>(load_height);

  switch (load_channels) {
    case 4:
      image->format = ImageFormat::RGBA;
      break;
    case 3:
      image->format = ImageFormat::RGB;
      break;
    default:
      image->format = ImageFormat::Invalid;
  }

  // TODO(colintan): Do checked arithmetic
  uint32_t img_size = image->width * image->height * static_cast<uint32_t>(load_channels);
  image->pixels = std::vector<uint8_t>(stb_pixels, stb_pixels + img_size);

  return image;
}

} // namespace