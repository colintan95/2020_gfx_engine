#ifndef RESOURCE_RESOURCE_GAL_H_
#define RESOURCE_RESOURCE_GAL_H_

#include <cassert>
#include <cstdint>
#include <variant>
#include "gal/object.h"
#include "resource/resource.h"

namespace gal {
class GALPlatform;
} // namespace

namespace resource {

class ResourceGAL : public ResourceBase {
friend class ResourceManagerGAL;

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
  ResourceGAL(Type type);
  ~ResourceGAL();

  ResourceGAL(const ResourceGAL&) = delete;
  ResourceGAL& operator=(const ResourceGAL&) = delete;

  ResourceGAL(ResourceGAL&& other);
  ResourceGAL& operator=(ResourceGAL&& other);

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

class HandleGAL : public HandleBase {
public:
  HandleGAL();
  HandleGAL(ResourceManagerBase* manager, ResourceBase* base);
  ~HandleGAL();

  HandleGAL(const HandleGAL&) = delete;
  HandleGAL& operator=(const HandleGAL&) = delete;

  HandleGAL(HandleGAL&& other);
  HandleGAL& operator=(HandleGAL&& other);

  template<typename T>
  T& Get() { return GetInternal<T>(); }

private:
  template<typename T>
  T& GetInternal() {
    return resource_->Get<T>();
  }

private:
  ResourceGAL* resource_;
};


} // namespace

#endif // RESOURCE_RESOURCE_GAL_H_