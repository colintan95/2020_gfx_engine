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
  // TODO(colintan): Should inherit from an interface class
  class Builder {
  friend class GALPipelineImplVk;

  private:
    using ConcreteType = GALPipelineBase<GALPipelineImplVk, Builder>;

  public:
    struct Viewport {
      float x = 0.f;
      float y = 0.f;
      float width = 0.f;
      float height = 0.f;
    };

    struct VertexInput {
      int buffer_idx = 0;
      int stride = 0;
    };

    struct VertexDesc {
      int buffer_idx = 0;
      int shader_idx = 0;
      int num_components = 0;
      int offset = 0;
    };

  public:
    Builder(GALPlatform* gal_platform) : gal_platform_(gal_platform) {}

    Builder& SetShader(ShaderType type, const GALShader& shader);
    Builder& SetViewport(const Viewport& viewport);
    Builder& AddVertexInput(const VertexInput& vert_input);
    Builder& AddVertexDesc(const VertexDesc& vert_desc);
    
    ConcreteType Create();

  private:
    GALPlatform* gal_platform_;

    GALShader vert_shader_;
    GALShader frag_shader_;

    Viewport viewport_;

    std::vector<VertexInput> vert_inputs_;
    std::vector<VertexDesc> vert_descs_;
  };

private:
  void CreateFromBuilder(Builder& builder);

public:
  void Destroy() final;

  VkPipeline GetPipeline() const { return vk_pipeline_; }

  VkRenderPass GetRenderPass() const { return vk_render_pass_; }
  const std::vector<VkFramebuffer>& GetFramebuffers() const { return vk_framebuffers_; }

private:
  VkPipelineLayout vk_pipeline_layout_;
  VkRenderPass vk_render_pass_;
  VkPipeline vk_pipeline_;

  std::vector<VkFramebuffer> vk_framebuffers_;
  
  VkDevice vk_device_;
};

using GALPipeline = GALPipelineBase<GALPipelineImplVk, GALPipelineImplVk::Builder>;
using GALPipelineBuilder = GALPipelineImplVk::Builder;

} // namespace 

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_