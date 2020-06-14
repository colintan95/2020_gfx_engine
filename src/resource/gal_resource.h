#ifndef RESOURCE_GAL_RESOURCE_H_
#define RESOURCE_GAL_RESOURCE_H_

#include <cassert>
#include <cstdint>
#include <variant>
#include "gal/object.h"
#include "resource/resource.h"

namespace gal {
class GALPlatform;
} // namespace

namespace resource {

class GALResource : public ResourceBase {
friend class GALResourceManager;

public:
  enum class Type { Buffer, Texture };

  struct BufferConfig {
    gal::BufferType type;
    uint8_t* data;
    size_t size;
  };

  struct TextureConfig {
    gal::TextureType type;
    gal::TextureFormat format;
    uint16_t width;
    uint16_t height;
    uint8_t* data;
  };

  using ConfigVariant = std::variant<BufferConfig, TextureConfig>;

public:
  GALResource(Type type);
  ~GALResource();

  GALResource(const GALResource&) = delete;
  GALResource& operator=(const GALResource&) = delete;

  GALResource(GALResource&& other);
  GALResource& operator=(GALResource&& other);

  void Alloc() final;
  void Dealloc() final;

  template<typename T>
  T& Get() {
    assert(std::holds_alternative<T>(resource_));
    return std::get<T>(resource_);
  }

private:
  using Variant = std::variant<gal::GALBuffer, gal::GALTexture>;

private:
  Type type_;
  Variant resource_;
};

class GALHandle : public HandleBase {
public:
  GALHandle();
  GALHandle(ResourceManagerBase* manager, ResourceBase* base);
  ~GALHandle();

  GALHandle(const GALHandle&) = delete;
  GALHandle& operator=(const GALHandle&) = delete;

  GALHandle(GALHandle&& other);
  GALHandle& operator=(GALHandle&& other);

  template<typename T>
  T& Get() { return GetInternal<T>(); }

private:
  template<typename T>
  T& GetInternal() {
    return resource_->Get<T>();
  }

private:
  GALResource* resource_;
};


} // namespace

#endif // RESOURCE_GAL_RESOURCE_H_