#ifndef GAL_COMMANDS_H_
#define GAL_COMMANDS_H_

#include <cstdint>
#include <variant>
#include "objects.h"

namespace gal {
namespace command {


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
};

struct DrawTriangles {
  uint32_t num_triangles;
};

using CommandUnion = 
    std::variant<
        SetPipeline,
        SetVertexDesc,
        SetUniformBuffer,
        SetVertexBuffer,
        DrawTriangles>;

} // namespace
} // namespace

#endif // GAL_COMMANDS_H_