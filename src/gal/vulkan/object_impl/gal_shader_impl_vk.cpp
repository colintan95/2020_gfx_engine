#include "gal/vulkan/object_impl/gal_shader_impl_vk.h"

// TODO(colintan): See if it's possible to just include gal/gal_platform.h instead of the
// platform specific header here
#include "gal/vulkan/gal_platform_vk.h"

namespace gal {

bool GALShaderImplVk::Create(GALPlatform* gal_platform, ShaderType type, 
                             const std::string& source) {
  return false;
}

bool GALShaderImplVk::CreateFromBinary(GALPlatform* gal_platform, ShaderType type, 
                                       const std::vector<std::byte>& shader_binary) {
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = shader_binary.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(shader_binary.data());

  vk_device_ = gal_platform->GetPlatformDetails()->vk_device;
  if (vkCreateShaderModule(vk_device_, &create_info, nullptr, &vk_shader_) != VK_SUCCESS) {
    return false;
  }

  return true;
}

void GALShaderImplVk::Destroy() {
  vkDestroyShaderModule(vk_device_, vk_shader_, nullptr);
}

} // namespace