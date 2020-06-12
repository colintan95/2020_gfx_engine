#ifndef GAL_COMMANDS_H_
#define GAL_COMMANDS_H_

#include <glm/glm.hpp>

#include <cstdint>
#include <variant>
#include "gal/objects.h"

namespace gal {
  
namespace command {

struct SetViewport {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};

// TODO(colintan): May need to be removed - vulkan may not support
struct ClearScreen {
  glm::vec4 color;
  bool clear_color = false;
  bool clear_depth = false;
};

struct SetPipeline {
  GALPipeline pipeline;
};

struct SetVertexDesc {
  GALVertexDesc vert_desc;
};

struct SetTextureSampler {
  GALTextureSampler sampler;
  uint8_t idx;
};

struct SetUniformBuffer {
  GALBuffer buffer;
  uint8_t idx; // Equivalent of opengl bindings
};

struct SetVertexBuffer {
  GALVertexBuffer buffer;
  uint8_t vert_idx; // Index of vertex in the vertex description
};

struct DrawTriangles {
  uint32_t num_triangles;
};

using CommandUnion = 
    std::variant<
        SetViewport,
        ClearScreen,
        SetPipeline,
        SetVertexDesc,
        SetTextureSampler,
        SetUniformBuffer,
        SetVertexBuffer,
        DrawTriangles>;
} // namespace

// TODO(colintan): Consider adding a Start() and End() function to be called before and after
// adding commands - helps safeguard against the user accidentally adding more commands to a
// command buffer that they have already considered finished
class GALCommandBuffer {
public:
  template<typename T>
  void Add(T command) {
    Entry entry;
    entry.cmd = command;
    entries_.push_back(entry);
  }

public:
  struct Entry {
    command::CommandUnion cmd;

    template<typename T>
    bool IsType() const { return std::holds_alternative<T>(cmd); }

    template<typename T>
    const T& AsType() const { return std::get<T>(cmd); }
  };

public:
  std::vector<Entry> entries_;
};

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf);

} // namespace

#endif // GAL_COMMANDS_H_