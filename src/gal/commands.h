#ifndef GAL_COMMANDS_H_
#define GAL_COMMANDS_H_

#include <cstdint>
#include <variant>
#include "objects.h"

namespace gal {
namespace command {

enum class CommandType {
  Invalid,
  SetPipeline,
  SetVertexDesc,
  SetUniformBuffer,
  SetVertexBuffer,
  DrawTriangles
};

struct GALCommand {
  virtual CommandType Type() const = 0;
}; 

struct SetPipeline : public GALCommand {
  GALPipeline pipeline;

  CommandType Type() const final { return CommandType::SetPipeline; }
};

struct SetVertexDesc : public GALCommand {
  GALVertexDesc vert_desc;

  CommandType Type() const final { return CommandType::SetVertexDesc; }
};

struct SetUniformBuffer : public GALCommand {
  CommandType Type() const final { return CommandType::SetUniformBuffer; }
};

struct SetVertexBuffer : public GALCommand {
  GALVertexBuffer buffer;

  CommandType Type() const final { return CommandType::SetVertexBuffer; }
};

struct DrawTriangles : public GALCommand {
  uint32_t num_triangles;

  CommandType Type() const final { return CommandType::DrawTriangles; }
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