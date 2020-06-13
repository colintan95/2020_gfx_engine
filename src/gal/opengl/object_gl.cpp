#include "gal/object.h"

#include <GL/glew.h>

#include <iostream>
#include <utility>
#include "gal/opengl/id_converter.h"
#include "gal/opengl/platform_gl.h"

namespace gal {

namespace {

bool CheckShaderSuccess(int shader) {
  int gl_success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(shader, 256, nullptr, log);

    // TODO(colintan): Do better logging
    std::cerr << "Failed to compile shader." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

bool CheckProgramSuccess(int program) {
  int gl_success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(program, 256, nullptr, log);

    // TODO(colintan): Do better logging
    std::cerr << "Failed to link program." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

} // namespace

std::optional<GALShader> GALShader::Create(GALPlatform* platform, ShaderType type, 
                                           const std::string& source) {
  GLuint gl_shader;
  switch (type) {
  case ShaderType::Vertex:
    gl_shader = glCreateShader(GL_VERTEX_SHADER);
    break;
  case ShaderType::Fragment:
    gl_shader = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  default:
    return std::nullopt;
  }

  const char* sources[] = { source.c_str() };
  glShaderSource(gl_shader, 1, sources, nullptr);
  glCompileShader(gl_shader);
  if (!CheckShaderSuccess(gl_shader)) {
    std::exit(EXIT_FAILURE);
  }
  

  GALShader result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_shader);
  result.type = type;
  return result;
}

GALShader::~GALShader() {
  if (IsLastRef()) {
    if (auto gl_shader_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteShader(*gl_shader_opt);
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

std::optional<GALPipeline> GALPipeline::Create(GALPlatform* platform, GALShader vert_shader, 
                                               GALShader frag_shader) {
  GLuint gl_program = glCreateProgram();

  if (auto vert_shader_opt = platform->GetPlatformDetails()->ConvertGALId(vert_shader.GetGALId())) {
    glAttachShader(gl_program, *vert_shader_opt);
  } else {
    return std::nullopt;
  }
  if (auto frag_shader_opt = platform->GetPlatformDetails()->ConvertGALId(frag_shader.GetGALId())) {
    glAttachShader(gl_program, *frag_shader_opt);
  } else {
    return std::nullopt;
  }
  
  glLinkProgram(gl_program);
  if (!CheckProgramSuccess(gl_program)) {
    return std::nullopt;
  }

  GALPipeline result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_program);

  return result;
}

GALPipeline::~GALPipeline() {
  if (IsLastRef()) {
    if (auto gl_program_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteProgram(*gl_program_opt);
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

std::optional<GALVertexBuffer> GALVertexBuffer::Create(GALPlatform* platform, uint8_t* data, 
                                                       size_t num_bytes) {
  GLuint gl_vbo;
  glCreateBuffers(1, &gl_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  glBufferData(GL_ARRAY_BUFFER, num_bytes, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GALVertexBuffer result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_vbo);

  return result;
}

GALVertexBuffer::~GALVertexBuffer() {
  if (IsLastRef()) {
    if (auto gl_vbo_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteBuffers(1, &(*gl_vbo_opt));
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

std::optional<GALVertexDesc> GALVertexDesc::Create(GALPlatform* platform) {
  GLuint gl_vao;
  glCreateVertexArrays(1, &gl_vao);

  GALVertexDesc result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_vao);

  return result;
}

GALVertexDesc::~GALVertexDesc() {
  if (IsLastRef()) {
    if (auto gl_vao_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteBuffers(1, &(*gl_vao_opt));
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

void GALVertexDesc::SetAttribute(uint8_t index, uint8_t size) {
  Entry entry;
  entry.index = index;
  entry.size = size;
  entries.push_back(std::move(entry));
}

std::optional<GALBuffer> GALBuffer::Create(GALPlatform* platform, BufferType type, uint8_t* data, 
                                           size_t size) {
  GLuint gl_buf;
  glCreateBuffers(1, &gl_buf);
  
  GLuint buf_target;
  switch (type) {
  case BufferType::Vertex:
    buf_target = GL_ARRAY_BUFFER;
    break;
  case BufferType::Uniform:
    buf_target = GL_UNIFORM_BUFFER;
    break;
  default:
    return std::nullopt;
  }

  glBindBuffer(buf_target, gl_buf);
  glBufferData(buf_target, size, data, GL_STATIC_DRAW);
  glBindBuffer(buf_target, 0);
    
  GALBuffer result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_buf);
  result.type_ = type;
  result.size_ = size;

  return result;
}

GALBuffer::~GALBuffer() {
  if (IsLastRef()) {
    if (auto gl_buf_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteBuffers(1, &(*gl_buf_opt));
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

// bool GALBuffer::Update(uint8_t* data, size_t start_idx, size_t update_size) {
//   if ((start_idx + update_size) > size_) {
//     return false;
//   }
  
//   // TODO(colintan): Implement
// }

std::optional<GALTexture> GALTexture::Create(GALPlatform* platform, TextureType type, 
                                             TextureFormat format, uint16_t width, uint16_t height, 
                                             uint8_t* data) {
  GLuint gl_tex;
  glGenTextures(1, &gl_tex);

  if (type == TextureType::Texture2D && format == TextureFormat::RGB) {
    glBindTexture(GL_TEXTURE_2D, gl_tex);

    // TODO(colintan): Expose these as parameters to user
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  } else {
    return std::nullopt;
  }

  GALTexture result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_tex);
  result.type_ = type;
  result.format_ = format;
  result.width_ = width;
  result.height_ = height;
  
  return result;
}

GALTexture::~GALTexture() {
  if (IsLastRef()) {
    if (auto gl_tex_opt = platform_details_->ConvertGALId(GetGALId())) {
      glDeleteTextures(1, &(*gl_tex_opt));
      platform_details_->RemoveGALId(GetGALId());
    }
  }
}

std::optional<GALTextureSampler> GALTextureSampler::Create(GALPlatform* platform, 
                                                           const GALTexture& texture) {
  // TODO(colintan): Replace this with something better
  static GLuint unit_counter = 0;

  std::optional<GLuint> gl_tex_opt = 
      platform->GetPlatformDetails()->ConvertGALId(texture.GetGALId());
  if (!gl_tex_opt) {
    return std::nullopt;
  }

  ++unit_counter;
  GLuint gl_tex_unit = unit_counter;
  glActiveTexture(GL_TEXTURE0 + gl_tex_unit);
  if (texture.GetType() == TextureType::Texture2D) {
    glBindTexture(GL_TEXTURE_2D, *gl_tex_opt);
  } else {
    return std::nullopt;
  }

  GALTextureSampler result{platform};
  platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_tex_unit);
  result.texture_ = texture;

  return result;
}

GALTextureSampler::~GALTextureSampler() {}

} // namespace