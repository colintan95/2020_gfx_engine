#include "gal/commands.h"

#include <GL/glew.h>

#include <iostream>
#include <unordered_map>
#include "gal/objects.h"
#include "gal/opengl/id_converter.h"

namespace gal {

namespace {

// Temporary variables that lasts through the lifetime of the command buffer
struct TempState {
  // Maps a vertex index to its vertex description information
  std::unordered_map<uint8_t, GALVertexDesc::Entry> vert_desc_map;
};

void SetTextureSampler(const command::SetTextureSampler& cmd) {
  std::optional<GLuint> gl_sampler_opt = opengl::ConvertGALId(cmd.sampler.GetGALId());
  if (!gl_sampler_opt.has_value()) {
    return;
  }
  glUniform1i(cmd.idx, *gl_sampler_opt);
}

void SetUniformBuffer(const command::SetUniformBuffer& cmd) {
  if (std::optional<GLuint> gl_buf_opt = opengl::ConvertGALId(cmd.buffer.GetGALId())) {
    glBindBufferBase(GL_UNIFORM_BUFFER, cmd.idx, *gl_buf_opt);
  }
}

void SetVertexDesc(const command::SetVertexDesc& cmd, TempState& tmp_state) {
  if (std::optional<GLuint> gl_id_opt = opengl::ConvertGALId(cmd.vert_desc.GetGALId())) {
    GLuint vao = *gl_id_opt;
    glBindVertexArray(vao);

    for (const GALVertexDesc::Entry& entry : *(cmd.vert_desc.entries)) {
      tmp_state.vert_desc_map[entry.index] = entry;
    }
  }
}

void SetVertexBuffer(const command::SetVertexBuffer& cmd, TempState& tmp_state) {
  if (std::optional<GLuint> gl_id_opt = opengl::ConvertGALId(cmd.buffer.GetGALId())) {
    GLuint vbo = *gl_id_opt;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    auto desc_map_it = tmp_state.vert_desc_map.find(cmd.vert_idx);
    if (desc_map_it == tmp_state.vert_desc_map.end()) {
      // TODO(colintan): Log error
      return;
    }
    const GALVertexDesc::Entry& entry = desc_map_it->second;

    glVertexAttribPointer(entry.index, entry.size, GL_FLOAT, GL_FALSE, 
                          entry.size * sizeof(float), nullptr);
    glEnableVertexAttribArray(entry.index);
  }  
}

} // namespace

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  TempState tmp_state;

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

      if (std::optional<GLuint> gl_program_opt = opengl::ConvertGALId(cmd.pipeline.GetGALId())) {
        glUseProgram(*gl_program_opt);
      }
    } else if (entry.IsType<command::SetTextureSampler>()) {
      SetTextureSampler(entry.AsType<command::SetTextureSampler>());

    } else if (entry.IsType<command::SetUniformBuffer>()) {
      SetUniformBuffer(entry.AsType<command::SetUniformBuffer>());

    } else if (entry.IsType<command::SetVertexDesc>()) {
      SetVertexDesc(entry.AsType<command::SetVertexDesc>(), tmp_state);

    } else if (entry.IsType<command::SetVertexBuffer>()) {
      SetVertexBuffer(entry.AsType<command::SetVertexBuffer>(), tmp_state);

    } else if (entry.IsType<command::DrawTriangles>()) {
      auto cmd = entry.AsType<command::DrawTriangles>();

      glDrawArrays(GL_TRIANGLES, 0, cmd.num_triangles * 3);
    }
  }
}

} // namespace