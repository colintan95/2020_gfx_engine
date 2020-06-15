#include "render/renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include "gal/gal.h"
#include "resource/resource_gal.h"
#include "resource/resource_manager_gal.h"
#include "resource/image_loader.h"
#include "resource/resource_system.h"
#include "window/window.h"

namespace render {

namespace {

const char kVertShaderSrc[] =
    "#version 430 core\n"
    "layout(location = 0) in vec3 vert_pos;\n"
    "layout(location = 2) in vec2 vert_texcoord;\n"
    "out vec2 frag_texcoord;\n"
    "layout(std140, binding = 0) uniform Matrices {\n"
    "  mat4 model_mat;\n"
    "  mat4 view_mat;\n"
    "  mat4 proj_mat;\n"
    "};\n"
    "void main() {\n"
    "  frag_texcoord = vert_texcoord;\n"
    "  gl_Position = proj_mat * view_mat * model_mat * vec4(vert_pos, 1.0);\n"
    "}";
const char kFragShaderSrc[] =
    "#version 430 core\n"
    "in vec2 frag_texcoord;\n"
    "out vec4 out_color;\n"
    "layout(location = 1) uniform sampler2D tex_sampler;\n"
    "void main() {\n"
    "  out_color = texture(tex_sampler, frag_texcoord);\n"
    "}";

} // namespace

Renderer::Renderer() {}

Renderer::~Renderer() {}

bool Renderer::Initialize(window::Window* window, resource::ResourceSystem* resource_system) {
  window_ = window;
  resource_system_ = resource_system;

  gal_platform_ = std::make_unique<gal::GALPlatform>();
  if (!gal_platform_->Initialize()) {
    std::cerr << "Failed to initialize gal platform." << std::endl;
    return false;
  }

  resource_manager_ = std::make_unique<resource::ResourceManagerGAL>(gal_platform_.get());

  resource::Handle<resource::Model> model_handle =
      resource_system_->LoadModel("assets/cube/cube.obj");
  if (!model_handle.IsValid()) {
    std::cerr << "Failed to load model." << std::endl;
    return false;
  }
  resource::Model& model = model_handle.Get();

  resource::Handle<resource::Image> image_handle = 
    resource_system_->LoadImage("assets/cube/default.png");
  if (!image_handle.IsValid()) {
    std::cerr << "Failed to load image." << std::endl;
    return false;
  }
  resource::Image& image = image_handle.Get();

  command_buffer_.SetPlatform(gal_platform_.get());

  gal::command::SetViewport set_viewport;
  set_viewport.x = 0;
  set_viewport.y = 0;
  set_viewport.width = window_->GetWidth();
  set_viewport.height = window_->GetHeight();
  command_buffer_.Add(set_viewport);

  auto vert_shader_opt = 
      gal_platform_->Create<gal::GALShader>(gal::ShaderType::Vertex, kVertShaderSrc);
  if (!vert_shader_opt) {
    std::cerr << "Failed to create GAL vertex shader." << std::endl;
    return false;
  }
  auto frag_shader_opt = 
      gal_platform_->Create<gal::GALShader>(gal::ShaderType::Fragment, kFragShaderSrc);
  if (!frag_shader_opt) {
    std::cerr << "Failed to create GAL fragment shader." << std::endl;
    return false;
  }
  auto pipeline_opt = 
      gal_platform_->Create<gal::GALPipeline>(*vert_shader_opt, *frag_shader_opt);
  if (!pipeline_opt) {
    std::cerr << "Failed to create GAL pipeline." << std::endl;
    return false;
  }          

  gal::command::SetPipeline set_pipeline;
  set_pipeline.pipeline = *pipeline_opt;
  command_buffer_.Add(set_pipeline);

  struct {
    glm::mat4 model_mat;
    glm::mat4 view_mat;
    glm::mat4 proj_mat;
  } uniform_data;

  float aspect_ratio = 
    static_cast<float>(window_->GetWidth()) / static_cast<float>(window_->GetHeight());
  uniform_data.model_mat = glm::mat4{1.f};
  uniform_data.view_mat = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, -10.f}) *
      glm::rotate(glm::mat4{1.f}, glm::radians(-15.f), glm::vec3{0.f, 1.f, 0.f});
  uniform_data.proj_mat = glm::perspective(glm::radians(30.f), aspect_ratio, 0.1f, 1000.f);

  resource::HandleGALBuffer uniform_buf_handle = 
      resource_manager_->CreateBuffer(gal::BufferType::Uniform, 
                                      reinterpret_cast<uint8_t*>(&uniform_data),
                                      sizeof(uniform_data));
  if (!uniform_buf_handle.IsValid()) {
    std::cerr << "Failed to create GAL buffer for uniforms." << std::endl;
    return false;
  }

  gal::command::SetUniformBuffer set_uniform_buf;
  set_uniform_buf.buffer = uniform_buf_handle.Get();
  set_uniform_buf.idx = 0;
  command_buffer_.Add(set_uniform_buf);
  
  resource::HandleGALTexture texture_handle =
      resource_manager_->CreateTexture(gal::TextureType::Texture2D, gal::TextureFormat::RGB,
                                       image.width, image.height, image.pixels.data());
  if (!texture_handle.IsValid()) {
    std::cerr << "Failed to create GAL texture." << std::endl;
    return false;
  }

  auto tex_sampler_opt = gal_platform_->Create<gal::GALTextureSampler>(texture_handle.Get());
  if (!tex_sampler_opt) {
    std::cerr << "Failed to create GAL texture sampler." << std::endl;
    return false;
  }

  gal::command::SetTextureSampler set_tex_sampler;
  set_tex_sampler.sampler = *tex_sampler_opt;
  set_tex_sampler.idx = 1;
  command_buffer_.Add(set_tex_sampler);           

  auto vert_desc_opt = gal_platform_->Create<gal::GALVertexDesc>();
  if (!vert_desc_opt) {
    std::cerr << "Failed to create GAL vertex description." << std::endl;
    return false;
  }
  vert_desc_opt->SetAttribute(0, 3);
  vert_desc_opt->SetAttribute(2, 2);
  
  gal::command::SetVertexDesc set_vert_desc;
  set_vert_desc.vert_desc = *vert_desc_opt;
  command_buffer_.Add(set_vert_desc);

  resource::HandleGALBuffer pos_buf_handle = 
      resource_manager_->CreateBuffer(gal::BufferType::Vertex, 
                                      reinterpret_cast<uint8_t*>(model.positions.data()),
                                      model.positions.size() * sizeof(glm::vec3));
  if (!pos_buf_handle.IsValid()) {
    std::cerr << "Failed to create GAL vertex buffer for positions." << std::endl;
    return false;
  }

  gal::command::SetVertexBuffer set_pos_vert_buf;
  set_pos_vert_buf.buffer = pos_buf_handle.Get();
  set_pos_vert_buf.vert_idx = 0;
  command_buffer_.Add(set_pos_vert_buf);

  resource::HandleGALBuffer texcoord_buf_handle = 
      resource_manager_->CreateBuffer(gal::BufferType::Vertex, 
                                      reinterpret_cast<uint8_t*>(model.texcoords.data()),
                                      model.texcoords.size() * sizeof(glm::vec2));
  if (!texcoord_buf_handle.IsValid()) {
    std::cerr << "Failed to create GAL vertex buffer for texcoords." << std::endl;
    return false;
  }

  gal::command::SetVertexBuffer set_texcoord_vert_buf;
  set_texcoord_vert_buf.buffer = texcoord_buf_handle.Get();
  set_texcoord_vert_buf.vert_idx = 2;
  command_buffer_.Add(set_texcoord_vert_buf);
  
  gal::command::ClearScreen clear_screen;
  clear_screen.color = glm::vec4{0.f, 0.f, 0.f, 1.f};
  clear_screen.clear_color = true;
  clear_screen.clear_depth = true;
  command_buffer_.Add(clear_screen);

  gal::command::DrawTriangles draw_triangles;
  draw_triangles.num_triangles = model.faces;
  command_buffer_.Add(draw_triangles);

  return true;
}

void Renderer::Cleanup() {
  resource_manager_.release();
  gal_platform_->Cleanup();

  resource_system_ = nullptr;
  window_ = nullptr;
}

void Renderer::Tick() {
  gal_platform_->ExecuteCommandBuffer(command_buffer_);
}

} // namespace