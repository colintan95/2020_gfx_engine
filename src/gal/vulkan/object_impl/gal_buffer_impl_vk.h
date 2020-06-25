#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_BUFFER_IMPL_VK_H_
#define GAL_VULKAN_OBJECT_IMPL_GAL_BUFFER_IMPL_VK_H_

#include <vulkan/vulkan.h>

#include <optional>
#include "gal/object_impl/gal_buffer_impl.h"

namespace gal {

class GALBufferImplVk : public IGALBufferImpl {
friend class Builder;

public:
  class Builder {
  friend class GALBufferImplVk;

  private:
    using ConcreteType = GALBufferBase<GALBufferImplVk, Builder>;

  public:
    Builder(GALPlatform* gal_platform) : gal_platform_(gal_platform) {}

    Builder& SetType(BufferType type);
    Builder& SetBufferData(uint8_t* data, size_t size);

    ConcreteType Create();

  private:
    GALPlatform* gal_platform_;

    BufferType buffer_type_;
    uint8_t* data_;
    size_t data_size_;
  };

private:
  struct BufferInfo {
    VkBuffer vk_buffer;
    VkDeviceMemory vk_buffer_memory;
  };

private:
  void CreateFromBuilder(Builder& builder);

  std::optional<BufferInfo> CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, 
                                         VkMemoryPropertyFlags properties);

public:
  void Destroy() final;

  VkBuffer GetBuffer() const { return vk_buffer_; }

private:
  VkPhysicalDevice vk_physical_device_;
  VkDevice vk_device_;
  VkBuffer vk_buffer_;
  VkDeviceMemory vk_buffer_memory_;
}; 

using GALBuffer = GALBufferBase<GALBufferImplVk, GALBufferImplVk::Builder>;

} // namespace gal

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_BUFFER_IMPL_VK_H_