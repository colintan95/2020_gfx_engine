#include "gal/vulkan/object_impl/gal_command_buffer_impl_vk.h"

#include <iostream>
#include <variant>
#include "gal/commands.h"
#include "gal/gal_pipeline.h"
#include "gal/vulkan/gal_platform_vk.h"

namespace gal {
   
GALCommandBufferImplVk::Builder::ReturnType GALCommandBufferImplVk::Builder::Create() {
  ReturnType res;
  res.GetImpl().CreateFromBuilder(*this);
  res.SetValid(true);
  return res;
}

void GALCommandBufferImplVk::CreateFromBuilder(GALCommandBufferImplVk::Builder& builder) {
  gal_platform_ = builder.gal_platform_;
  vk_device_ = builder.gal_platform_->GetPlatformDetails()->vk_device;

  uint32_t framebuffer_count = 
      builder.gal_platform_->GetPlatformDetails()->vk_swapchain_image_views.size();

  vk_command_buffers_.resize(framebuffer_count);

  VkCommandBufferAllocateInfo command_buffer_alloc_info{};
  command_buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_alloc_info.commandPool = 
      builder.gal_platform_->GetPlatformDetails()->vk_command_pool;
  command_buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_alloc_info.commandBufferCount = static_cast<uint32_t>(vk_command_buffers_.size());

  if (vkAllocateCommandBuffers(vk_device_, &command_buffer_alloc_info, 
                               vk_command_buffers_.data()) != VK_SUCCESS) {
    std::cerr << "Could not create command buffers." << std::endl;
    throw Builder::ReturnType::InitException();
  }
}

void GALCommandBufferImplVk::Destroy() {

}

void GALCommandBufferImplVk::SubmitCommand(const CommandVariant& command_variant) {
  if (std::holds_alternative<command::SetPipeline>(command_variant)) {
    const command::SetPipeline& command = std::get<command::SetPipeline>(command_variant);

    for (size_t i = 0; i < vk_command_buffers_.size(); ++i) {
      VkClearValue clear_color = {0.f, 0.f, 0.f, 1.f};

      VkRenderPassBeginInfo render_pass_begin_info{};
      render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      render_pass_begin_info.renderPass = command.pipeline.GetImpl().GetRenderPass();
      render_pass_begin_info.framebuffer = command.pipeline.GetImpl().GetFramebuffers()[i];
      render_pass_begin_info.renderArea.offset = {0, 0};
      render_pass_begin_info.renderArea.extent = 
          gal_platform_->GetPlatformDetails()->vk_swapchain_extent;
      render_pass_begin_info.clearValueCount = 1;
      render_pass_begin_info.pClearValues = &clear_color;

      vkCmdBeginRenderPass(vk_command_buffers_[i], &render_pass_begin_info, 
                           VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(vk_command_buffers_[i], VK_PIPELINE_BIND_POINT_GRAPHICS, 
                        command.pipeline.GetImpl().GetPipeline());
    }
  } else if (std::holds_alternative<command::DrawTriangles>(command_variant)) {
    const command::DrawTriangles& command = std::get<command::DrawTriangles>(command_variant);

    for (VkCommandBuffer command_buffer : vk_command_buffers_) {
      vkCmdDraw(command_buffer, 3, command.num_triangles, 0, 0);
    }
  }
}

bool GALCommandBufferImplVk::BeginRecording() {
  for (VkCommandBuffer command_buffer : vk_command_buffers_) {
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
      std::cerr << "Could not begin command buffer." << std::endl;
      return false;
    }
  }
  return true;
}

bool GALCommandBufferImplVk::EndRecording() {
  for (VkCommandBuffer command_buffer : vk_command_buffers_) {
    vkCmdEndRenderPass(command_buffer);

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
      std::cerr << "Could not end command buffer." << std::endl;
      return false;
    }
  }
  return true;
}

} // namespace