#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_VK_H_
#define GAL_VULKAN_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_VK_H_

#include <vulkan/vulkan.h>

#include <vector>
#include "gal/object_impl/gal_command_buffer_impl.h"

namespace gal {

class GALCommandBufferImplVk : public IGALCommandBufferImpl {
friend class Builder;

public:
  class Builder {
  friend class GALCommandBufferImplVk;

  private:
    using ReturnType = GALCommandBufferBase<GALCommandBufferImplVk, Builder>;

  public:
    Builder(GALPlatform* gal_platform) : gal_platform_(gal_platform) {}

    ReturnType Create();

  private:
    GALPlatform* gal_platform_;
  };

public:
  void Destroy() final;

  void SubmitCommand(const CommandVariant& command_variant) final;

  bool BeginRecording() final;
  bool EndRecording() final;

private:
  void CreateFromBuilder(Builder& builder);

private:
  GALPlatform* gal_platform_;

  VkDevice vk_device_;
  std::vector<VkCommandBuffer> vk_command_buffers_;
};

using GALCommandBuffer = 
    GALCommandBufferBase<GALCommandBufferImplVk, GALCommandBufferImplVk::Builder>;

} // namespace

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_VK_H_