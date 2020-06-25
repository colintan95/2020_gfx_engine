#include "gal/vulkan/object_impl/gal_buffer_impl_vk.h"

#include <cstring>
#include <iostream>
#include <optional>
#include <vulkan/vulkan.h>
#include "gal/vulkan/gal_platform_vk.h"

namespace gal {

GALBufferImplVk::Builder& GALBufferImplVk::Builder::SetType(BufferType type) {
  buffer_type_ = type;
  return *this;
}

GALBufferImplVk::Builder& 
    GALBufferImplVk::Builder::SetBufferData(uint8_t* data, size_t size) {
  data_ = data;
  data_size_ = size;
  return *this;
}

GALBufferImplVk::Builder::ConcreteType GALBufferImplVk::Builder::Create() {
  ConcreteType res;
  res.GetImpl().CreateFromBuilder(*this);
  res.SetValid(true);
  return res;
}

void GALBufferImplVk::CreateFromBuilder(GALBufferImplVk::Builder& builder) {
  vk_device_ = builder.gal_platform_->GetPlatformDetails()->vk_device;
  vk_physical_device_ = 
      builder.gal_platform_->GetPlatformDetails()->vk_physical_device;

  std::optional<BufferInfo> vert_buf_info_opt;

  if (builder.buffer_type_ == BufferType::Vertex) {
    vert_buf_info_opt = CreateBuffer(builder.data_size_, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  } else {
    std::cerr << "Buffer type not supported." << std::endl;
    throw Builder::ConcreteType::InitException();
  }
      
  if (!vert_buf_info_opt.has_value()) {
    std::cerr << "Could not create VkBuffer." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  vk_buffer_ = vert_buf_info_opt.value().vk_buffer;
  vk_buffer_memory_ = vert_buf_info_opt.value().vk_buffer_memory;

  void* device_data;
  vkMapMemory(vk_device_, vk_buffer_memory_, 0, builder.data_size_, 0, &device_data);
  memcpy(device_data, builder.data_, builder.data_size_);
  vkUnmapMemory(vk_device_, vk_buffer_memory_);
}

std::optional<GALBufferImplVk::BufferInfo> 
    GALBufferImplVk::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, 
                                  VkMemoryPropertyFlags properties) {
  VkBufferCreateInfo buffer_create_info{};
  buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_create_info.size = size;
  buffer_create_info.usage = usage;
  buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  BufferInfo buffer_info{};

  if (vkCreateBuffer(vk_device_, &buffer_create_info, nullptr, &buffer_info.vk_buffer) 
          != VK_SUCCESS) {
    std::cerr << "Could not create VkBuffer." << std::endl;
    return std::nullopt;
  }

  VkMemoryRequirements memory_req;
  vkGetBufferMemoryRequirements(vk_device_, buffer_info.vk_buffer, &memory_req);

  VkPhysicalDeviceMemoryProperties memory_props;
  vkGetPhysicalDeviceMemoryProperties(vk_physical_device_, &memory_props);

  bool found_memory_type = false;
  uint32_t memory_type_index = 0;

  for (uint32_t i = 0; i < memory_props.memoryTypeCount; ++i) {
    if (memory_req.memoryTypeBits & (1 << i) && 
        memory_props.memoryTypes[i].propertyFlags == properties) {
      found_memory_type = true;
      memory_type_index = i;
    }
  }

  if (!found_memory_type) {
    std::cerr << "Could not find memory for memory type." << std::endl;
    return std::nullopt;
  }

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = memory_req.size;
  alloc_info.memoryTypeIndex = memory_type_index;

  if (vkAllocateMemory(vk_device_, &alloc_info, nullptr, &buffer_info.vk_buffer_memory) 
          != VK_SUCCESS) {
    std::cerr << "Could not allocate VkDeviceMemory." << std::endl;
    return std::nullopt;
  }

  vkBindBufferMemory(vk_device_, buffer_info.vk_buffer, buffer_info.vk_buffer_memory, 0);

  return buffer_info;
}

void GALBufferImplVk::Destroy() {
  vkFreeMemory(vk_device_, vk_buffer_memory_, nullptr);
  vkDestroyBuffer(vk_device_, vk_buffer_, nullptr);
}

} // namespace gal