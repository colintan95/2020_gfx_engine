#ifndef GAL_COMMAND_H_
#define GAL_COMMAND_H_

#include <glm/glm.hpp>

#include <cstdint>
#include <utility>
#include <variant>
#include <vector>
// #include "gal/gal_buffer.h"
#include "gal/gal_pipeline.h"
// #include "gal/gal_texture_sampler.h"

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

// struct SetTextureSampler {
//   GALTextureSampler sampler;
//   // TODO(colintan): Rename to something clearer - e.g. uniform_idx? but without uniform in it
//   uint8_t idx;
// };

// struct SetUniformBuffer {
//   GALBuffer buffer;
//   uint8_t idx; // Equivalent of opengl bindings
// };

// struct SetVertexBuffer {
//   GALBuffer buffer;
//   uint8_t vert_idx; // Index of vertex in the vertex description
// };

struct DrawTriangles {
  uint32_t num_triangles;
};
    
} // namespace

using CommandVariant = 
    std::variant<
        command::SetViewport,
        command::ClearScreen,
        command::SetPipeline,
        // SetTextureSampler,
        // SetUniformBuffer,
        // SetVertexBuffer,
        command::DrawTriangles>;

} // namespace

#endif // GAL_COMMAND_H_