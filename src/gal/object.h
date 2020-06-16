#ifndef GAL_OBJECT_H_
#define GAL_OBJECT_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "gal/gal_texture.h"

namespace gal {

class GALPlatform;

namespace internal {
class PlatformDetails;
} // namespace

using GALId = uint32_t;

enum class ShaderType {
  Invalid,
  Vertex,
  Fragment
};

enum class ObjectType {
  Shader,
  Pipeline,
  VertexDesc,
  Buffer,
  Texture,
  TextureSampler
};

// TODO(colintan): Make abstract base class
class GALObject {
public:
  GALObject(GALPlatform* platform);
  GALObject() {}
  virtual ~GALObject() {};

  GALId GetGALId() const { return *gal_id_; }

  bool IsLastRef() const { return gal_id_.use_count() == 1; }

protected:
  GALPlatform* GetPlatform() { return gal_platform_; }

protected:
  internal::PlatformDetails* platform_details_;

private:
  std::shared_ptr<GALId> gal_id_;
  GALPlatform* gal_platform_;
};

class GALShader : public GALObject {
public:
  GALShader(GALPlatform* platform) : GALObject(platform) {}
  GALShader() {}
  ~GALShader();

  static ObjectType GetObjectType() { return ObjectType::Shader; }

  static std::optional<GALShader> Create(GALPlatform* platform, ShaderType type, 
                                         const std::string& source);

private:
  ShaderType type;
};

class GALPipeline : public GALObject {
public:
  GALPipeline(GALPlatform* platform) : GALObject(platform) {}
  GALPipeline() {}
  ~GALPipeline();

  static ObjectType GetObjectType() { return ObjectType::Pipeline; }

  static std::optional<GALPipeline> Create(GALPlatform* platform, GALShader vert_shader, 
                                           GALShader frag_shader);
};

// TODO(colintan): See if this is too big to be passed around by value
class GALVertexDesc : public GALObject {
public:
  GALVertexDesc(GALPlatform* platform) : GALObject(platform) {}
  GALVertexDesc() {}
  ~GALVertexDesc();

  void SetAttribute(uint8_t index, uint8_t size);

  static ObjectType GetObjectType() { return ObjectType::VertexDesc; }

  static std::optional<GALVertexDesc> Create(GALPlatform* platform);

  struct Entry {
    uint8_t index;
    uint8_t size;
    // TODO(colintan): Add more fields
  };

  // TODO(colintan): Make this private?
  // TODO(colintan): May be expensive to copy. Change to something else that's more pass-by-value
  // friendly
  std::vector<Entry> entries;
};

// class GALTexture : public GALObject {
// public:
//   GALTexture(GALPlatform* platform) : GALObject(platform) {}
//   GALTexture() {}
//   ~GALTexture();

//   static ObjectType GetObjectType() { return ObjectType::Texture; }

//   static std::optional<GALTexture> Create(GALPlatform* platform, TextureType type, 
//                                           TextureFormat format, uint16_t width, uint16_t height, 
//                                           uint8_t* data);

//   // TODO(colintan): Change this to GetType()
//   TextureType GetType() const { return type_; }

// private:
//   TextureType type_;
//   TextureFormat format_;
//   uint16_t width_;
//   uint16_t height_;
// };

class GALTextureSampler : public GALObject {
public:
  GALTextureSampler(GALPlatform* platform) : GALObject(platform) {}
  GALTextureSampler() {}
  ~GALTextureSampler();

  static ObjectType GetObjectType() { return ObjectType::TextureSampler; }

  static std::optional<GALTextureSampler> Create(GALPlatform* platform, const GALTexture& texture);

private:
  GALTexture texture_;
};

} // namespace

#endif // GAL_OBJECT_H_