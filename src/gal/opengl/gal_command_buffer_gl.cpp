#include "gal/gal_command_buffer.h"

#include <GL/glew.h>

#include <iostream>
#include <unordered_map>
#include "gal/opengl/gal_platform_gl.h"

namespace gal {

namespace {

// Temporary variables that lasts through the lifetime of the command buffer
struct TempState {
  // Maps a vertex index to its vertex description information
  std::unordered_map<uint8_t, GALPipeline::VertexDesc::Entry> vert_desc_map;
};

class CommandExecutor {
public:
  CommandExecutor(internal::PlatformDetails* platform_details) {
    platform_details_ = platform_details;
  }

  void SetPipeline(const command::SetPipeline& cmd) {
    glUseProgram(cmd.pipeline.GetImpl().GetProgramGLId());

    glBindVertexArray(cmd.pipeline.GetImpl().GetVaoGLId());

    for (const auto& entry : cmd.pipeline.GetVertexDesc().GetEntries()) {
      temp_state_.vert_desc_map[entry.index] = entry;
    }
  }

  void SetTextureSampler(const command::SetTextureSampler& cmd) {
    glUniform1i(cmd.idx, cmd.sampler.GetImpl().GetTexUnit());
  }

  void SetUniformBuffer(const command::SetUniformBuffer& cmd) {
    glBindBufferBase(GL_UNIFORM_BUFFER, cmd.idx, cmd.buffer.GetImpl().GetGLId());
  }

  void SetVertexBuffer(const command::SetVertexBuffer& cmd) {
    glBindBuffer(GL_ARRAY_BUFFER, cmd.buffer.GetImpl().GetGLId());

    auto desc_map_it = temp_state_.vert_desc_map.find(cmd.vert_idx);
    if (desc_map_it == temp_state_.vert_desc_map.end()) {
      // TODO(colintan): Log error
      return;
    }
    const GALPipeline::VertexDesc::Entry& entry = desc_map_it->second;

    glVertexAttribPointer(entry.index, entry.size, GL_FLOAT, GL_FALSE, 
                          entry.size * sizeof(float), nullptr);
    glEnableVertexAttribArray(entry.index); 
  }

private:
  TempState temp_state_;
  internal::PlatformDetails* platform_details_;
};

} // namespace

void GALCommandBuffer::Execute() const {
  CommandExecutor executor(platform_->GetPlatformDetails());

  for (const GALCommandBuffer::Entry& entry : entries_) {

    // TODO(colintan): Consider using std::visit to iterate through variant

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
      executor.SetPipeline(entry.AsType<command::SetPipeline>());

    } else if (entry.IsType<command::SetTextureSampler>()) {
      executor.SetTextureSampler(entry.AsType<command::SetTextureSampler>());

    } else if (entry.IsType<command::SetUniformBuffer>()) {
      executor.SetUniformBuffer(entry.AsType<command::SetUniformBuffer>());

    // } else if (entry.IsType<command::SetVertexDesc>()) {
    //   executor.SetVertexDesc(entry.AsType<command::SetVertexDesc>());

    } else if (entry.IsType<command::SetVertexBuffer>()) {
      executor.SetVertexBuffer(entry.AsType<command::SetVertexBuffer>());

    } else if (entry.IsType<command::DrawTriangles>()) {
      auto cmd = entry.AsType<command::DrawTriangles>();

      glDrawArrays(GL_TRIANGLES, 0, cmd.num_triangles * 3);
    }
  }
}

} // namespace