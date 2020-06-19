#ifndef GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_
#define GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_

#include <vulkan/vulkan.h>

#include "gal/gal_shader.h"
#include "gal/object_impl/gal_pipeline_impl.h"

namespace gal {

class GALPipelineImplVk : public IGALPipelineImpl {
friend class Builder;

public:
  class Builder {
  private:
    using ReturnType = GALPipelineBase<GALPipelineImplVk, Builder>;

  public:
    Builder(GALPlatform* gal_platform) : gal_platform_(gal_platform) {}

    ReturnType Create() {
      ReturnType res;
      if (!res.Create(gal_platform_, vert_shader_, frag_shader_)) {
        throw ReturnType::InitException();
      }

      return res;
    }

    Builder& SetShader(ShaderType type, const GALShader& shader) {
      switch (type) {
      case ShaderType::Vertex:
        vert_shader_ = shader;
        break;
      case ShaderType::Fragment:
        frag_shader_ = shader;
        break;
      default:
        throw ReturnType::InitException();
      }

      return *this;
    }

  private:
    GALPlatform* gal_platform_;
    GALShader vert_shader_;
    GALShader frag_shader_;
  };

public:
  bool Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
              const GALShader& frag_shader) final;
  void Destroy() final;

private:

};

using GALPipeline = GALPipelineBase<GALPipelineImplVk, GALPipelineImplVk::Builder>;

} // namespace 

#endif // GAL_VULKAN_OBJECT_IMPL_GAL_PIPELINE_IMPL_VK_