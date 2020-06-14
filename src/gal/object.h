#ifndef GAL_OBJECT_H_
#define GAL_OBJECT_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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

enum class BufferType {
  Vertex,
  Uniform
};

// TODO(colintan): Make this an abstract base class
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

// TODO(colintan): Deal with destruction of graphics API objects
class GALShader : public GALObject {
public:
  GALShader(GALPlatform* platform) : GALObject(platform) {}
  GALShader() {}
  ~GALShader();

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

  static std::optional<GALPipeline> Create(GALPlatform* platform, GALShader vert_shader, 
                                           GALShader frag_shader);
};

class GALVertexBuffer : public GALObject {
public:
  GALVertexBuffer(GALPlatform* platform) : GALObject(platform) {}
  GALVertexBuffer() {}
  ~GALVertexBuffer();   

  static std::optional<GALVertexBuffer> Create(GALPlatform* platform, uint8_t* data, 
                                               size_t num_bytes);                                      
};

// TODO(colintan): See if this is too big to be passed around by value
class GALVertexDesc : public GALObject{
public:
  GALVertexDesc(GALPlatform* platform) : GALObject(platform) {}
  GALVertexDesc() {}
  ~GALVertexDesc();

  void SetAttribute(uint8_t index, uint8_t size);

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

class GALBuffer : public GALObject {
public:
  GALBuffer(GALPlatform* platform) : GALObject(platform) {}
  GALBuffer() {}
  ~GALBuffer();

  static std::optional<GALBuffer> Create(GALPlatform* platform, BufferType type, uint8_t* data, 
                                         size_t size);

  // bool Update(uint8_t* data, size_t start_idx, size_t update_size);

  BufferType GetType() { return type_; }
  size_t GetSize() { return size_; }

private:
  BufferType type_;
  size_t size_;
};

enum class TextureType {
  Texture2D,
  Texture3D
};

enum class TextureFormat {
  Invalid,
  RGB,
  RGBA
};

class GALTexture : public GALObject {
public:
  GALTexture(GALPlatform* platform) : GALObject(platform) {}
  GALTexture() {}
  ~GALTexture();

  static std::optional<GALTexture> Create(GALPlatform* platform, TextureType type, 
                                          TextureFormat format, uint16_t width, uint16_t height, 
                                          uint8_t* data);

  TextureType GetType() const { return type_; }

private:
  TextureType type_;
  TextureFormat format_;
  uint16_t width_;
  uint16_t height_;
};

class GALTextureSampler : public GALObject {
public:
  GALTextureSampler(GALPlatform* platform) : GALObject(platform) {}
  GALTextureSampler() {}
  ~GALTextureSampler();

  static std::optional<GALTextureSampler> Create(GALPlatform* platform, const GALTexture& texture);

private:
  GALTexture texture_;
};

} // namespace

#endif // GAL_OBJECT_H_