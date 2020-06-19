#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_
#define GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_

#include <vulkan/vulkan.h>

#include "gal/object_impl/gal_pipeline_impl.h"

namespace gal {

class GALPipelineImplVk : public IGALPipelineImpl {
public:
  bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
              const GALShader& frag_shader) final;
  void Destroy() final;
};

using GALPipeline = GALPipelineBase<GALPipelineImplVk>;

} // namespace 

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_