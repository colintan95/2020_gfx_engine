#include "resource/resource_manager_gal.h"

#include <utility>
#include <memory>
#include <variant>
#include "gal/gal.h"

namespace resource {

ResourceManagerGAL::ResourceManagerGAL(gal::GALPlatform* gal_platform) 
  : gal_platform_(gal_platform) {

}

ResourceManagerGAL::~ResourceManagerGAL() {

}

HandleGAL ResourceManagerGAL::CreateResource(ResourceGAL::Type type, 
                                             ResourceGAL::ConfigVariant config) {
  std::unique_ptr<ResourceGAL> resource = std::make_unique<ResourceGAL>(type);

  if (type == ResourceGAL::Type::Buffer) {

    ResourceGAL::BufferConfig* buffer_config = std::get_if<ResourceGAL::BufferConfig>(&config);
    if (buffer_config == nullptr) {
      return HandleGAL();
    }

    std::optional<gal::GALBuffer> opt = 
        gal::GALBuffer::Create(gal_platform_, 
                               buffer_config->type, 
                               buffer_config->data, 
                               buffer_config->size);
    if (!opt.has_value()) {
      return HandleGAL();
    }
    resource->resource_ = std::move(*opt);

  } else if (type == ResourceGAL::Type::Texture) {

    ResourceGAL::TextureConfig* texture_config = std::get_if<ResourceGAL::TextureConfig>(&config);
    if (texture_config == nullptr) {
      return HandleGAL();
    }

    std::optional<gal::GALTexture> opt = 
        gal::GALTexture::Create(gal_platform_, 
                                texture_config->type, 
                                texture_config->format,
                                texture_config->width, 
                                texture_config->height, 
                                texture_config->data);
    if (!opt.has_value()) {
      return HandleGAL();
    }
    resource->resource_ = std::move(*opt);

  } else {
    return HandleGAL();
  }

  HandleGAL handle = CreateHandle<HandleGAL>(resource.get());
  resources_.push_back(std::move(resource));

  return handle;
}

} // namespace