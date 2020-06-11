#include "objects.h"

#include <cstdint>
#include <optional>
#include <string>
#include "internal/interface.h"

namespace gal {

std::optional<GALPipeline> GALPipeline::Create(GALShader vert_shader, GALShader frag_shader) {
  return internal::CreatePipeline(vert_shader, frag_shader);
}

std::optional<GALShader> GALShader::Create(ShaderType type, const std::string& source) {
  return internal::CreateShader(type, source);
}

std::optional<GALVertexDesc> GALVertexDesc::Create() {
  return internal::CreateVertexDesc();
}

std::optional<GALVertexBuffer> GALVertexBuffer::Create(uint8_t* data, size_t num_bytes) {
  return internal::CreateVertexBuffer(data, num_bytes);
}


} // namespace