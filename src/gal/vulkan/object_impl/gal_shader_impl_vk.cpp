#include "gal/vulkan/object_impl/gal_shader_impl_vk.h"

namespace gal {

bool GALShaderImplVk::Create(GALPlatform* gal_platform, ShaderType type, 
                             const std::string& source) {
  return true;
}

void GALShaderImplVk::Destroy() {
  
}

} // namespace