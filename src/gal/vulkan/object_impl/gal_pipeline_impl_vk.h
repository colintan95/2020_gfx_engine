#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_
#define GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_

#include <vulkan/vulkan.h>

#include <vector>
#include "gal/gal_shader.h"
#include "gal/object_impl/gal_pipeline_impl.h"

namespace gal {

class GALPipelineImplVk : public IGALPipelineImpl {
friend class Builder;

public:
  class Builder {
  friend class GALPipelineImplVk;

  private:
    using ReturnType = GALPipelineBase<GALPipelineImplVk, Builder>;

    struct Viewport {
      float x = 0.f;
      float y = 0.f;
      float width = 0.f;
      float height = 0.f;
    };

  public:
    Builder(GALPlatform* gal_platform) : gal_platform_(gal_platform) {}

    Builder& SetShader(ShaderType type, const GALShader& shader);
    Builder& SetViewport(float x, float y, float width, float height);
    
    ReturnType Create();

  private:
    GALPlatform* gal_platform_;
    GALShader vert_shader_;
    GALShader frag_shader_;
    Viewport viewport_;
  };

private:
  void CreateFromBuilder(Builder& builder);

public:
  void Destroy() final;

private:
  VkPipelineLayout vk_pipeline_layout_;
  VkRenderPass vk_render_pass_;
  VkPipeline vk_pipeline_;

  std::vector<VkFramebuffer> vk_framebuffers_;
  
  VkDevice vk_device_;
};

using GALPipeline = GALPipelineBase<GALPipelineImplVk, GALPipelineImplVk::Builder>;

} // namespace 

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_