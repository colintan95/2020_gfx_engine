#ifndef GAL_INTERNAL_INTERFACE_H_
#define GAL_INTERNAL_INTERFACE_H_

#include <cstdint>
#include <optional>
#include <string>
#include "../objects.h"
#include "../command_buffer.h"

namespace gal {
namespace internal {

std::optional<GALPipeline> CreatePipeline(GALShader vert_shader, GALShader frag_shader);

std::optional<GALShader> CreateShader(ShaderType type, const std::string& source);

std::optional<GALVertexDesc> CreateVertexDesc();

std::optional<GALVertexBuffer> CreateVertexBuffer(uint8_t* data, size_t num_bytes);

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf);

} // namespace
} // namespace

#endif // GAL_INTERNAL_INTERFACE_H_