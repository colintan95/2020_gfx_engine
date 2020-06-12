#ifndef GAL_OBJECTS_H_
#define GAL_OBJECTS_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace gal {

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
  GALObject();
  virtual ~GALObject() {};

  GALId GetGALId() const { return gal_id_; }

private:
  GALId gal_id_;
};

// TODO(colintan): Deal with destruction of graphics API objects
class GALShader : public GALObject {
public:
  static std::optional<GALShader> Create(ShaderType type, const std::string& source);

private:
  ShaderType type;
};

class GALPipeline : public GALObject {
public:
  static std::optional<GALPipeline> Create(GALShader vert_shader, GALShader frag_shader);
};

class GALVertexBuffer : public GALObject {
public:
  static std::optional<GALVertexBuffer> Create(uint8_t* data, size_t num_bytes);
};

class GALVertexDesc : public GALObject{
public:
  static std::optional<GALVertexDesc> Create();

  struct Entry {
    uint8_t index;
    uint8_t size;
    // TODO(colintan): Add more fields
  };

  std::shared_ptr<std::vector<Entry>> entries;
};

class GALBuffer : public GALObject {
public:
  static std::optional<GALBuffer> Create(BufferType type, uint8_t* data, size_t size);

  // bool Update(uint8_t* data, size_t start_idx, size_t update_size);

  BufferType GetType() { return type_; }
  size_t GetSize() { return size_; }

private:
  BufferType type_;
  size_t size_;
};

} // namespace

#endif // GAL_OBJECTS_H_