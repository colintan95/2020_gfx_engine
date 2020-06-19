#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_SHADER_IMPL_VK_
#define GAL_VULKAN_OBJECT_IMPL_GAL_SHADER_IMPL_VK_

#include <vulkan/vulkan.h>

#include "gal/object_impl/gal_shader_impl.h"

namespace gal {

class GALShaderImplVk : public IGALShaderImpl {
public:
  bool Create(GALPlatform* gal_platform, ShaderType type, const std::string& source) final;

  bool CreateFromBinary(GALPlatform* gal_platform, ShaderType type, 
                        const std::vector<std::byte>& shader_binary) final;

  void Destroy() final;

  VkShaderModule GetShaderModule() const { return vk_shader_; }

private:
  VkShaderModule vk_shader_;
  VkDevice vk_device_;
};

using GALShader = GALShaderBase<GALShaderImplVk>;

} // namespace

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_SHADER_IMPL_VK_