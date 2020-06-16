#include "gal/object.h"

#include <GL/glew.h>

#include <iostream>
#include <utility>
#include "gal/opengl/id_converter.h"
#include "gal/gal_shader.h"
#include "gal/opengl/gal_platform_gl.h"

namespace gal {

// std::optional<GALShader> GALShader::Create(GALPlatform* platform, ShaderType type, 
//                                            const std::string& source) {
//   GLuint gl_shader;
//   switch (type) {
//   case ShaderType::Vertex:
//     gl_shader = glCreateShader(GL_VERTEX_SHADER);
//     break;
//   case ShaderType::Fragment:
//     gl_shader = glCreateShader(GL_FRAGMENT_SHADER);
//     break;
//   default:
//     return std::nullopt;
//   }

//   const char* sources[] = { source.c_str() };
//   glShaderSource(gl_shader, 1, sources, nullptr);
//   glCompileShader(gl_shader);
//   if (!CheckShaderSuccess(gl_shader)) {
//     std::exit(EXIT_FAILURE);
//   }
  

//   GALShader result{platform};
//   platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_shader);
//   result.type = type;
//   return result;
// }

// GALShader::~GALShader() {
//   if (IsLastRef()) {
//     if (auto gl_shader_opt = platform_details_->ConvertGALId(GetGALId())) {
//       glDeleteShader(*gl_shader_opt);
//       platform_details_->RemoveGALId(GetGALId());
//     }
//   }
// }

// std::optional<GALPipeline> GALPipeline::Create(GALPlatform* platform, GALShader vert_shader, 
//                                                GALShader frag_shader) {
//   GLuint gl_program = glCreateProgram();

//   // TODO(colintan): Check that shaders are valid
//   glAttachShader(gl_program, vert_shader.GetImpl().GetGLId());
//   glAttachShader(gl_program, frag_shader.GetImpl().GetGLId());
  
//   glLinkProgram(gl_program);
//   if (!CheckProgramSuccess(gl_program)) {
//     return std::nullopt;
//   }

//   GALPipeline result{platform};
//   platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_program);

//   return result;
// }

// GALPipeline::~GALPipeline() {
//   if (IsLastRef()) {
//     if (auto gl_program_opt = platform_details_->ConvertGALId(GetGALId())) {
//       glDeleteProgram(*gl_program_opt);
//       platform_details_->RemoveGALId(GetGALId());
//     }
//   }
// }

// std::optional<GALVertexDesc> GALVertexDesc::Create(GALPlatform* platform) {
//   GLuint gl_vao;
//   glCreateVertexArrays(1, &gl_vao);

//   GALVertexDesc result{platform};
//   platform->GetPlatformDetails()->AddGALId(result.GetGALId(), gl_vao);

//   return result;
// }

// GALVertexDesc::~GALVertexDesc() {
//   if (IsLastRef()) {
//     if (auto gl_vao_opt = platform_details_->ConvertGALId(GetGALId())) {
//       glDeleteBuffers(1, &(*gl_vao_opt));
//       platform_details_->RemoveGALId(GetGALId());
//     }
//   }
// }

// void GALVertexDesc::SetAttribute(uint8_t index, uint8_t size) {
//   Entry entry;
//   entry.index = index;
//   entry.size = size;
//   entries.push_back(std::move(entry));
// }

std::optional<GALTextureSampler> GALTextureSampler::Create(GALPlatform* platform, 
                                                           const GALTexture& texture) {
  // TODO(colintan): Replace this with something better
  static GLuint unit_counter = 0;

  // TODO(colintan): Check that GALTexture returns a valid GLuint
  GLuint gl_tex = texture.GetImpl().GetGLId();

  ++unit_counter;
  GLuint gl_tex_unit = unit_counter;
  glActiveTexture(GL_TEXTURE0 + gl_tex_unit);
  if (texture.GetType() == TextureType::Texture2D) {
    glBindTexture(GL_TEXTURE_2D, gl_tex);
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