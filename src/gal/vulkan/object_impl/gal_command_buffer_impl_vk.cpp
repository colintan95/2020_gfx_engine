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

void GALCommandBufferImplVk::SubmitCommand(const CommandVariant& command) {
  if (std::holds_alternative<command::SetPipeline>(command)) {

  }
}

bool GALCommandBufferImplVk::BeginRecording() {
  return true;
}

bool GALCommandBufferImplVk::EndRecording() {
  return true;
}

} // namespace