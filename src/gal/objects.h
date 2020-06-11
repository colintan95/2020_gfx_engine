#ifndef GAL_OBJECTS_H_
#define GAL_OBJECTS_H_

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <string>

namespace gal {

using GALId = uint32_t;

enum class ShaderType {
  Invalid,
  Vertex,
  Fragment
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
    GALVertexBuffer buffer;
    uint8_t index;
    uint8_t size;
    // TODO(colintan): Add more fields
  };

  // TODO(colintan): Support more entries
  std::array<Entry, 1> entries;

  Entry& Index(int i) {
    assert(i == 0);
    return entries[i];
  }

  
};

} // namespace

#endif // GAL_OBJECTS_H_