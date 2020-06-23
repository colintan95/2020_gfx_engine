#include "gal/vulkan/object_impl/gal_pipeline_impl_vk.h"

#include <iostream>
#include <utility>
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
    throw ConcreteType::InitException();
  }

  return *this;
}

GALPipelineImplVk::Builder& 
    GALPipelineImplVk::Builder::SetViewport(const Viewport& viewport) {
  viewport_ = viewport;
  return *this;
}

GALPipelineImplVk::Builder&
    GALPipelineImplVk::Builder::AddVertexInput(const VertexInput& vert_input) {
  vert_inputs_.push_back(vert_input);
  return *this;
}

GALPipelineImplVk::Builder&
    GALPipelineImplVk::Builder::AddVertexDesc(const VertexDesc& vert_desc) {
  vert_descs_.push_back(vert_desc);
  return *this;
}

GALPipelineImplVk::Builder::ConcreteType GALPipelineImplVk::Builder::Create() {
  ConcreteType res;
  res.GetImpl().CreateFromBuilder(*this);
  res.SetValid(true);
  return res;
}

void GALPipelineImplVk::Destroy() {
  for (VkFramebuffer framebuffer : vk_framebuffers_) {
    vkDestroyFramebuffer(vk_device_, framebuffer, nullptr);
  }

  vkDestroyPipeline(vk_device_, vk_pipeline_, nullptr);
  vkDestroyRenderPass(vk_device_, vk_render_pass_, nullptr);
  vkDestroyPipelineLayout(vk_device_, vk_pipeline_layout_, nullptr);
}

void GALPipelineImplVk::CreateFromBuilder(GALPipelineImplVk::Builder& builder) {
  vk_device_ = builder.gal_platform_->GetPlatformDetails()->vk_device;

  VkPipelineShaderStageCreateInfo vert_shader_stage{};
  vert_shader_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage.module = builder.vert_shader_.GetImpl().GetShaderModule();
  vert_shader_stage.pName = "main";

  VkPipelineShaderStageCreateInfo frag_shader_stage{};
  frag_shader_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage.module = builder.frag_shader_.GetImpl().GetShaderModule();
  frag_shader_stage.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage, frag_shader_stage };

  std::vector<VkVertexInputBindingDescription> vert_binding_descs;
  for (const GALPipelineImplVk::Builder::VertexInput& vert_input : builder.vert_inputs_) {
    VkVertexInputBindingDescription desc;
    desc.binding = vert_input.buffer_idx;
    desc.stride = vert_input.stride;
    desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vert_binding_descs.push_back(std::move(desc));
  }

  std::vector<VkVertexInputAttributeDescription> vert_attribute_descs;
  for (const GALPipelineImplVk::Builder::VertexDesc& vert_desc : builder.vert_descs_) {
    VkVertexInputAttributeDescription desc;
    desc.binding = vert_desc.buffer_idx;
    desc.location = vert_desc.shader_idx;
    
    switch (vert_desc.num_components) {
    case 2:
      desc.format = VK_FORMAT_R32G32_SFLOAT;
      break;
    case 3:
      desc.format = VK_FORMAT_R32G32B32_SFLOAT;
      break;
    default: 
      std::cerr << "Vertex format not supported." << std::endl;
      throw Builder::ConcreteType::InitException();
    }

    desc.offset = vert_desc.offset;
    vert_attribute_descs.push_back(std::move(desc));
  }

  VkPipelineVertexInputStateCreateInfo vert_input_state{};
  vert_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vert_input_state.vertexBindingDescriptionCount = vert_binding_descs.size();
  vert_input_state.pVertexBindingDescriptions = vert_binding_descs.data();
  vert_input_state.vertexAttributeDescriptionCount = vert_attribute_descs.size();
  vert_input_state.pVertexAttributeDescriptions = vert_attribute_descs.data();

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state{};
  input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_state.primitiveRestartEnable = VK_FALSE;

  // TODO(colintan): Use the default viewport - the window dimensions - if user does not specify
  // an initial viewport. Also add support for changing the viewport later on (using Vulkan's
  // dynamic state))
  VkViewport viewport{};
  viewport.x = builder.viewport_.x;
  viewport.y = builder.viewport_.y;
  viewport.width = builder.viewport_.width;
  viewport.height = builder.viewport_.height;
  viewport.minDepth = 0.f;
  viewport.maxDepth = 1.f;

  VkExtent2D& swapchain_extent = builder.gal_platform_->GetPlatformDetails()->vk_swapchain_extent;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchain_extent;

  VkPipelineViewportStateCreateInfo viewport_state{};
  viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state.viewportCount = 1;
  viewport_state.pViewports = &viewport;
  viewport_state.scissorCount = 1;
  viewport_state.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterization_state{};
  rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterization_state.depthClampEnable = VK_FALSE;
  rasterization_state.rasterizerDiscardEnable = VK_FALSE;
  rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
  rasterization_state.lineWidth = 1.f;
  rasterization_state.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterization_state.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterization_state.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisample_state{};
  multisample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample_state.sampleShadingEnable = VK_FALSE;
  multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo color_blend_state{};
  color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend_state.logicOpEnable = VK_FALSE;
  color_blend_state.attachmentCount = 1;
  color_blend_state.pAttachments = &color_blend_attachment;

  VkPipelineLayoutCreateInfo  pipeline_layout_create_info{};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  
  if (vkCreatePipelineLayout(vk_device_, &pipeline_layout_create_info, nullptr,
                             &vk_pipeline_layout_) != VK_SUCCESS) {
    std::cerr << "Coult not create VkPipelineLayout." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  VkAttachmentDescription color_attachment{};
  color_attachment.format = builder.gal_platform_->GetPlatformDetails()->vk_swapchain_image_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref{};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkSubpassDependency subpass_dependency{};
  subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  subpass_dependency.dstSubpass = 0;
  subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpass_dependency.srcAccessMask = 0;
  subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_create_info{};
  render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_create_info.attachmentCount = 1;
  render_pass_create_info.pAttachments = &color_attachment;
  render_pass_create_info.subpassCount = 1;
  render_pass_create_info.pSubpasses = &subpass;
  render_pass_create_info.dependencyCount = 1;
  render_pass_create_info.pDependencies = &subpass_dependency;

  if (vkCreateRenderPass(vk_device_, &render_pass_create_info, nullptr, 
                         &vk_render_pass_) != VK_SUCCESS) {
    std::cerr << "Coult not create VkRenderPass." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  VkGraphicsPipelineCreateInfo pipeline_create_info{};
  pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = shader_stages;
  pipeline_create_info.pVertexInputState = &vert_input_state;
  pipeline_create_info.pInputAssemblyState = &input_assembly_state;
  pipeline_create_info.pViewportState = &viewport_state;
  pipeline_create_info.pRasterizationState = &rasterization_state;
  pipeline_create_info.pMultisampleState = &multisample_state;
  pipeline_create_info.pDepthStencilState = nullptr;
  pipeline_create_info.pColorBlendState = &color_blend_state;
  pipeline_create_info.pDynamicState = nullptr;
  pipeline_create_info.layout = vk_pipeline_layout_;
  pipeline_create_info.renderPass = vk_render_pass_;
  pipeline_create_info.subpass = 0;

  if (vkCreateGraphicsPipelines(vk_device_, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr,
                                &vk_pipeline_) != VK_SUCCESS) {
    std::cerr << "Coult not create VkPipeline." << std::endl;
    throw Builder::ConcreteType::InitException();
  }

  std::vector<VkImageView>& swapchain_image_views = 
      builder.gal_platform_->GetPlatformDetails()->vk_swapchain_image_views;

  vk_framebuffers_.resize(swapchain_image_views.size());
  for (size_t i = 0; i < swapchain_image_views.size(); ++i) {
    VkImageView attachments[] = { swapchain_image_views[i] };

    VkFramebufferCreateInfo framebuffer_create_info{};
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.renderPass = vk_render_pass_;
    framebuffer_create_info.attachmentCount = 1;
    framebuffer_create_info.pAttachments = attachments;
    framebuffer_create_info.width = swapchain_extent.width;
    framebuffer_create_info.height = swapchain_extent.height;
    framebuffer_create_info.layers = 1;

    if (vkCreateFramebuffer(vk_device_, &framebuffer_create_info, nullptr, 
                            &vk_framebuffers_[i]) != VK_SUCCESS) {
      std::cerr << "Coult not create VkFramebuffer." << std::endl;
      throw Builder::ConcreteType::InitException();
    }
  }
}

} // namespace