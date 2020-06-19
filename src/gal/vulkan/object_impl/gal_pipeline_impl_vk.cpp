#include "gal/vulkan/object_impl/gal_pipeline_impl_vk.h"

namespace gal {

bool GALPipelineImplVk::Create(GALPlatform* gal_platform, const GALShader& vert_shader, 
                               const GALShader& frag_shader) {
  VkPipelineShaderStageCreateInfo vert_shader_stage_create_info{};
  vert_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_create_info.module = vert_shader.GetImpl().GetShaderModule();
  vert_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_shader_stage_create_info{};
  frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_create_info.module = frag_shader.GetImpl().GetShaderModule();
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
  

  return true;
}

void GALPipelineImplVk::Destroy() {
  
}

} // namespace