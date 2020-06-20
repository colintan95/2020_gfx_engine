#include "gal/vulkan/object_impl/gal_pipeline_impl_vk.h"

#include "gal/vulkan/gal_platform_vk.h"

namespace gal {

GALPipelineImplVk::Builder& 
    GALPipelineImplVk::Builder::SetShader(ShaderType type, const GALShader& shader) {
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

GALPipelineImplVk::Builder& 
    GALPipelineImplVk::Builder::SetViewport(float x, float y, float width, float height) {
  viewport_.x = x;
  viewport_.y = y;
  viewport_.width = width;
  viewport_.height = height;

  return *this;
}

GALPipelineImplVk::Builder::ReturnType GALPipelineImplVk::Builder::Create() {
  ReturnType res;
  res.GetImpl().CreateFromBuilder(*this);
  res.SetValid(true);
  return res;
}

void GALPipelineImplVk::Destroy() {
  
}

void GALPipelineImplVk::CreateFromBuilder(GALPipelineImplVk::Builder& builder) {
  VkPipelineShaderStageCreateInfo vert_shader_stage_create_info{};
  vert_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_create_info.module = builder.vert_shader_.GetImpl().GetShaderModule();
  vert_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_shader_stage_create_info{};
  frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_create_info.module = builder.frag_shader_.GetImpl().GetShaderModule();
  frag_shader_stage_create_info.pName = "main";

  VkPipelineVertexInputStateCreateInfo vert_input_stage_create_info{};
  vert_input_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vert_input_stage_create_info.vertexBindingDescriptionCount = 0;
  vert_input_stage_create_info.pVertexBindingDescriptions = nullptr;
  vert_input_stage_create_info.vertexAttributeDescriptionCount = 0;
  vert_input_stage_create_info.pVertexAttributeDescriptions = nullptr;

  VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
  input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};
  viewport.x = builder.viewport_.x;
  viewport.y = builder.viewport_.y;
  viewport.width = builder.viewport_.width;
  viewport.height = builder.viewport_.height;
  viewport.minDepth = 0.f;
  viewport.maxDepth = 1.f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = builder.gal_platform_->GetPlatformDetails()->vk_swapchain_extent;

  VkPipelineViewportStateCreateInfo viewport_state_create_info{};
  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  viewport_state_create_info.pViewports = &viewport;
  viewport_state_create_info.scissorCount = 1;
  viewport_state_create_info.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
  rasterization_state_create_info.sType = 
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization_state_create_info.depthClampEnable = VK_FALSE;
  rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization_state_create_info.lineWidth = 1.f;
  rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state_create_info.depthBiasEnable = VK_FALSE;
  rasterization_state_create_info.depthBiasConstantFactor = 0.f;
  rasterization_state_create_info.depthBiasClamp = 0.f;
  rasterization_state_create_info.depthBiasSlopeFactor = 0.f;

  VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
  multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample_state_create_info.sampleShadingEnable = VK_FALSE;
  multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisample_state_create_info.minSampleShading = 1.f;
  multisample_state_create_info.pSampleMask = nullptr;
  multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
  multisample_state_create_info.alphaToOneEnable = VK_FALSE;

  // TODO(colintan): Add blending
}

} // namespace