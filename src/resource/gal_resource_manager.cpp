#include "resource/gal_resource_manager.h"

#include <utility>
#include <memory>
#include <variant>
#include "gal/gal.h"

namespace resource {

GALResourceManager::GALResourceManager(gal::GALPlatform* gal_platform) 
  : gal_platform_(gal_platform) {

}

GALResourceManager::~GALResourceManager() {

}

GALHandle GALResourceManager::CreateResource(GALResource::Type type, 
                                             GALResource::ConfigVariant config) {
  std::unique_ptr<GALResource> resource = std::make_unique<GALResource>(type);

  if (type == GALResource::Type::Buffer) {

    GALResource::BufferConfig* buffer_config = std::get_if<GALResource::BufferConfig>(&config);
    if (buffer_config == nullptr) {
      return GALHandle();
    }

    std::optional<gal::GALBuffer> opt = 
        gal::GALBuffer::Create(gal_platform_, 
                               buffer_config->type, 
                               buffer_config->data, 
                               buffer_config->size);
    if (!opt.has_value()) {
      return GALHandle();
    }
    resource->resource_ = std::move(*opt);

  } else if (type == GALResource::Type::Texture) {

    GALResource::TextureConfig* texture_config = std::get_if<GALResource::TextureConfig>(&config);
    if (texture_config == nullptr) {
      return GALHandle();
    }

    std::optional<gal::GALTexture> opt = 
        gal::GALTexture::Create(gal_platform_, 
                                texture_config->type, 
                                texture_config->format,
                                texture_config->width, 
                                texture_config->height, 
                                texture_config->data);
    if (!opt.has_value()) {
      return GALHandle();
    }
    resource->resource_ = std::move(*opt);

  } else {
    return GALHandle();
  }

  GALHandle handle = CreateHandle<GALHandle>(resource.get());
  resources_.push_back(std::move(resource));

  return handle;
}

} // namespace