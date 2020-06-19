#include "gal/vulkan/object_impl/gal_pipeline_impl_vk.h"

namespace gal {

bool GALPipelineImplVk::Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
                               const GALShader& frag_shader) {
  return true;
}

void GALPipelineImplVk::Destroy() {
  
}

} // namespace