#include "gal/gal_command_buffer.h"

#include <vulkan/vulkan.h>

#include <iostream>
#include <unordered_map>
#include "gal/vulkan/gal_platform_vk.h"

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
    
  }

  // void SetTextureSampler(const command::SetTextureSampler& cmd) {

  // }

  // void SetUniformBuffer(const command::SetUniformBuffer& cmd) {

  // }

  // void SetVertexBuffer(const command::SetVertexBuffer& cmd) {

  // }

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
      // glViewport(cmd.x, cmd.y, cmd.width, cmd.height);
    
    // } else if (entry.IsType<command::ClearScreen>()) {
    //   auto cmd = entry.AsType<command::ClearScreen>();

      // glClearColor(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
      // if (cmd.clear_color) {
      //   glClear(GL_COLOR_BUFFER_BIT);
      // }
      // if (cmd.clear_depth) {
      //   glClear(GL_DEPTH_BUFFER_BIT);
      // }

    } else if (entry.IsType<command::SetPipeline>()) {
      executor.SetPipeline(entry.AsType<command::SetPipeline>());

    // } else if (entry.IsType<command::SetTextureSampler>()) {
    //   executor.SetTextureSampler(entry.AsType<command::SetTextureSampler>());

    // } else if (entry.IsType<command::SetUniformBuffer>()) {
    //   executor.SetUniformBuffer(entry.AsType<command::SetUniformBuffer>());

    // } else if (entry.IsType<command::SetVertexBuffer>()) {
    //   executor.SetVertexBuffer(entry.AsType<command::SetVertexBuffer>());

    } else if (entry.IsType<command::DrawTriangles>()) {
      auto cmd = entry.AsType<command::DrawTriangles>();

      // glDrawArrays(GL_TRIANGLES, 0, cmd.num_triangles * 3);
    }
  }
}

} // namespace