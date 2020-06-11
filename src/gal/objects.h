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

// TODO(colintan): Make the constructor private. Consider making the fields private.
struct GALShader {
  GALId id;
  ShaderType type;

  static std::optional<GALShader> Create(ShaderType type, const std::string& source);
};

struct GALPipeline {
  // TODO(colintan): Change this to |gal_id|
  GALId id;

  static std::optional<GALPipeline> Create(GALShader vert_shader, GALShader frag_shader);
};

struct GALUniformBuffer {
  GALId id;

  static std::optional<GALUniformBuffer> Create();
};

struct GALVertexBuffer {
   GALId id;

  static std::optional<GALVertexBuffer> Create(uint8_t* data, size_t num_bytes);
};

struct GALVertexDesc {
  struct Entry {
    GALVertexBuffer buffer;
    uint8_t index;
    uint8_t size;
    // TODO(colintan): Add more fields
  };

  GALId id;
  // TODO(colintan): Support more entries
  std::array<Entry, 1> entries;

  Entry& Index(int i) {
    assert(i == 0);
    return entries[i];
  }

  static std::optional<GALVertexDesc> Create();
};

} // namespace

#endif // GAL_OBJECTS_H_