#ifndef GAL_COMMANDS_H_
#define GAL_COMMANDS_H_

#include <glm/glm.hpp>

#include <cstdint>
#include <variant>
#include "objects.h"

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

struct SetUniformBuffer {

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
        SetUniformBuffer,
        SetVertexBuffer,
        DrawTriangles>;

} // namespace
} // namespace

#endif // GAL_COMMANDS_H_