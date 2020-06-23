#include "gal/vulkan/object_impl/gal_buffer_impl_vk.h"

#include <cstring>
#include <iostream>
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

  VkBufferCreateInfo buffer_create_info{};
  buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_create_info.size = builder.data_size_;

  if (builder.buffer_type_ == BufferType::Vertex) {
    buffer_create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  } else {
    std::cerr << "Buffer type not supported." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  if (vkCreateBuffer(vk_device_, &buffer_create_info, nullptr, &vk_buffer_) != VK_SUCCESS) {
    std::cerr << "Could not create VkBuffer." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  VkMemoryRequirements memory_req;
  vkGetBufferMemoryRequirements(vk_device_, vk_buffer_, &memory_req);

  VkPhysicalDevice vk_physical_device = 
      builder.gal_platform_->GetPlatformDetails()->vk_physical_device;

  VkPhysicalDeviceMemoryProperties memory_props;
  vkGetPhysicalDeviceMemoryProperties(vk_physical_device, &memory_props);

  bool found_memory_type = false;
  uint32_t memory_type_index = 0;

  for (uint32_t i = 0; i < memory_props.memoryTypeCount; ++i) {
    if (memory_req.memoryTypeBits & (1 << i) && 
        memory_props.memoryTypes[i].propertyFlags == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                                                      VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
      found_memory_type = true;
      memory_type_index = i;
    }
  }

  if (!found_memory_type) {
    std::cerr << "Could not find memory for memory type." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = memory_req.size;
  alloc_info.memoryTypeIndex = memory_type_index;

  if (vkAllocateMemory(vk_device_, &alloc_info, nullptr, &vk_device_memory_) != VK_SUCCESS) {
    std::cerr << "Could not allocate VkDeviceMemory." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  vkBindBufferMemory(vk_device_, vk_buffer_, vk_device_memory_, 0);

  void* device_data;
  vkMapMemory(vk_device_, vk_device_memory_, 0, builder.data_size_, 0, &device_data);
  memcpy(device_data, builder.data_, builder.data_size_);
  vkUnmapMemory(vk_device_, vk_device_memory_);
}

void GALBufferImplVk::Destroy() {
  vkFreeMemory(vk_device_, vk_device_memory_, nullptr);
  vkDestroyBuffer(vk_device_, vk_buffer_, nullptr);
}

} // namespace gal