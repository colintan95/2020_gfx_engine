#include "../interface.h"

#include <GL/glew.h>

#include <iostream>
#include <optional>
#include <unordered_map>
#include "../../commands.h"
#include "../../command_buffer.h"
#include "../../objects.h"
#include "../../gal.h"

namespace gal {
namespace internal {

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

std::unordered_map<GALId, GLuint> gal_to_gl;

std::optional<GLuint> ConvertGALId(GALId id) {
  auto result_it = gal_to_gl.find(id);
  if (result_it == gal_to_gl.end()) {
    return std::nullopt;
  } else {
    return result_it->second;
  }
}

void AddGALId(GALId gal_id, GLuint gl_id) {
  gal_to_gl[gal_id] = gl_id;
}

GALId GenerateId() {
  static uint32_t counter = 0;

  ++counter;
  return GALId{counter};
}

} // namespace

std::optional<GALPipeline> CreatePipeline(GALShader vert_shader, GALShader frag_shader) {
  GLuint gl_program = glCreateProgram();

  if (auto vert_shader_opt = ConvertGALId(vert_shader.id)) {
    glAttachShader(gl_program, *vert_shader_opt);
  } else {
    return std::nullopt;
  }
  if (auto frag_shader_opt = ConvertGALId(frag_shader.id)) {
    glAttachShader(gl_program, *frag_shader_opt);
  } else {
    return std::nullopt;
  }
  
  glLinkProgram(gl_program);
  if (!CheckProgramSuccess(gl_program)) {
    return std::nullopt;
  }

  GALId gal_id = GenerateId();
  AddGALId(gal_id, gl_program);

  GALPipeline result;
  result.id = gal_id;

  return result;
}

std::optional<GALShader> CreateShader(ShaderType type, const std::string& source) {
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

  GALId gal_id = GenerateId();
  AddGALId(gal_id, gl_shader);

  GALShader result;
  result.id = gal_id;
  result.type = type;

  return result;
}

std::optional<GALVertexDesc> CreateVertexDesc() {
  GLuint gl_vao;
  glCreateVertexArrays(1, &gl_vao);

  GALId gal_id = GenerateId();
  AddGALId( gal_id, gl_vao);

  GALVertexDesc result;
  result.id = gal_id;

  return result;
}

std::optional<GALVertexBuffer> CreateVertexBuffer(uint8_t* data, size_t num_bytes) {
  GLuint gl_vbo;
  glCreateBuffers(1, &gl_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gl_vbo);
  std::cout << "No. of bytes: " << num_bytes << std::endl;
  glBufferData(GL_ARRAY_BUFFER, num_bytes, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GALId gal_id = GenerateId();
  AddGALId(gal_id, gl_vbo);

  GALVertexBuffer result;
  result.id = gal_id;

  return result;
}

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  for (const GALCommandBuffer::Entry& entry : cmd_buf.entries_) {

    if (entry.IsType<command::SetViewport>()) {
      auto cmd = entry.AsType<command::SetViewport>();
      glViewport(cmd.x, cmd.y, cmd.width, cmd.height);
    
    } else if (entry.IsType<command::ClearScreen>()) {
      auto cmd = entry.AsType<command::ClearScreen>();

      glClearColor(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
      if (cmd.clear_color) {
        glClear(GL_COLOR_BUFFER_BIT);
      }
      if (cmd.clear_depth) {
        glClear(GL_DEPTH_BUFFER_BIT);
      }

    } else if (entry.IsType<command::SetPipeline>()) {
      auto cmd = entry.AsType<command::SetPipeline>();

      if (std::optional<GLuint> gl_id_opt = ConvertGALId(cmd.pipeline.id)) {
        glUseProgram(*gl_id_opt);
      }

    } else if (entry.IsType<command::SetVertexDesc>()) {
      auto cmd = entry.AsType<command::SetVertexDesc>();

      if (std::optional<GLuint> gl_id_opt = ConvertGALId(cmd.vert_desc.id)) {
        GLuint gl_id = *gl_id_opt;
        glBindVertexArray(gl_id);

        // TODO(colintan): Support multiple entries
          const GALVertexDesc::Entry& entry = cmd.vert_desc.Index(0);
        if (std::optional<GLuint> gl_vbo_id_opt = ConvertGALId(entry.buffer.id)) {
          glBindBuffer(GL_ARRAY_BUFFER, *gl_vbo_id_opt);
          glVertexAttribPointer(entry.index, entry.size, GL_FLOAT, GL_FALSE, 
                                entry.size * sizeof(float), nullptr);
          glEnableVertexAttribArray(entry.index);
        }
      }
    } else if (entry.IsType<command::SetVertexBuffer>()) {
      auto cmd = entry.AsType<command::SetVertexBuffer>();
      
      if (std::optional<GLuint> gl_id_opt = ConvertGALId(cmd.buffer.id)) {
        glBindBuffer(GL_ARRAY_BUFFER, *gl_id_opt);
      }

    } else if (entry.IsType<command::DrawTriangles>()) {
      auto cmd = entry.AsType<command::DrawTriangles>();

      glDrawArrays(GL_TRIANGLES, 0, cmd.num_triangles * 3);
    }
  }
}

} // namespace
} // namespace