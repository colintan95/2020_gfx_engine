#include "render/renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <new>
#include <optional>
#include <utility>
#include <vector>
// #include "gal/gal_buffer.h"
#include "gal/gal_command_buffer.h"
#include "gal/gal_pipeline.h"
#include "gal/gal_shader.h"
// #include "gal/gal_texture_sampler.h"
// #include "resource/resource_gal.h"
// #include "resource/resource_manager_gal.h"
#include "resource/image_loader.h"
#include "resource/resource_system.h"
#include "window/window.h"

namespace {

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;
};

const std::vector<Vertex> kVertices = {
  {{0.f, -0.5f}, {1.f, 0.f, 0.f}},
  {{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
  {{-0.5f, 0.5f,}, {0.f, 0.f, 1.f}}
};

} // namespace

namespace render {

Renderer::Renderer(window::Window* window, resource::ResourceSystem* resource_system) {
  window_ = window;
  resource_system_ = resource_system;

  try {
    gal_platform_ = std::make_unique<gal::GALPlatform>(window_);
  } catch (gal::GALPlatform::InitException& e) {
    std::cerr << e.what() << std::endl;
    throw InitException();
  } catch (std::bad_alloc& ba) {
    throw InitException();
  }

  // TODO(colintan): Make this into a separate class
  std::ifstream vert_shader_file("shaders/triangle_vert.spv", std::ios::ate | std::ios::binary);
  if (!vert_shader_file.is_open()) {
    std::cerr << "Could not open vertex shader file." << std::endl;
    throw InitException();
  }

  size_t vert_shader_file_size = static_cast<size_t>(vert_shader_file.tellg());
  vert_shader_file.seekg(0);

  std::vector<std::byte> vert_shader_binary(vert_shader_file_size);
  vert_shader_file.read(reinterpret_cast<char*>(vert_shader_binary.data()), vert_shader_file_size);

  std::ifstream frag_shader_file("shaders/triangle_frag.spv", std::ios::ate | std::ios::binary);
  if (!frag_shader_file.is_open()) {
    std::cerr << "Could not open fragment shader file." << std::endl;
    throw InitException();
  }

  size_t frag_shader_file_size = static_cast<size_t>(frag_shader_file.tellg());
  frag_shader_file.seekg(0);

  std::vector<std::byte> frag_shader_binary(frag_shader_file_size);
  frag_shader_file.read(reinterpret_cast<char*>(frag_shader_binary.data()), frag_shader_file_size);

  gal::GALShader vert_shader;
  if (!vert_shader.CreateFromBinary(gal_platform_.get(), gal::ShaderType::Vertex, 
                                    vert_shader_binary)) {
    std::cerr << "Failed to create GAL vertex shader." << std::endl;
    throw InitException();
  }

  gal::GALShader frag_shader;
  if (!frag_shader.CreateFromBinary(gal_platform_.get(), gal::ShaderType::Fragment, 
                                    frag_shader_binary)) {
    std::cerr << "Failed to create GAL fragment shader." << std::endl;
    throw InitException();
  }

  gal::GALPipelineBuilder::Viewport viewport;
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = window_->GetWidth();
  viewport.height = window_->GetHeight();

  gal::GALPipelineBuilder::VertexInput vert_input;
  vert_input.buffer_idx = 0;
  vert_input.stride = 0;

  gal::GALPipelineBuilder::VertexDesc pos_desc;
  pos_desc.buffer_idx = 0;
  pos_desc.shader_idx = 0;
  pos_desc.num_components = 2;
  pos_desc.offset = 0;

  gal::GALPipelineBuilder::VertexDesc color_desc;
  color_desc.buffer_idx = 0;
  color_desc.shader_idx = 1;
  color_desc.num_components = 3;
  color_desc.offset = 2;
  
  try {
    pipeline_ = gal::GALPipeline::BeginBuild(gal_platform_.get())
        .SetShader(gal::ShaderType::Vertex, vert_shader)
        .SetShader(gal::ShaderType::Fragment, frag_shader)
        .SetViewport(viewport)
        .AddVertexInput(vert_input)
        .AddVertexDesc(pos_desc)
        .AddVertexDesc(color_desc)
        .Create();
  } catch (gal::GALPipeline::InitException& e) {
    std::cerr << e.what() << std::endl;
    throw InitException();
  }

  frag_shader.Destroy();
  vert_shader.Destroy();
  
  try {
    command_buffer_ = gal::GALCommandBuffer::BeginBuild(gal_platform_.get()).Create();
  } catch (gal::GALCommandBuffer::InitException& e) {
    std::cerr << e.what() << std::endl;
    throw InitException();
  }

  if (!command_buffer_.BeginRecording()) {
    std::cerr << "Command buffer could not begin recording." << std::endl;
    throw InitException();
  }

  gal::command::SetPipeline set_pipeline;
  set_pipeline.pipeline = pipeline_;
  command_buffer_.SubmitCommand(set_pipeline);

  gal::command::DrawTriangles draw_triangles;
  draw_triangles.num_triangles = 1;
  command_buffer_.SubmitCommand(draw_triangles);

  if (!command_buffer_.EndRecording()) {
    std::cerr << "Command buffer could not end recording." << std::endl;
    throw InitException();
  }

  // resource_manager_ = std::make_unique<resource::ResourceManagerGAL>(gal_platform_.get());

  // MeshId mesh_id;
  // if (std::optional<MeshId> mesh_id_opt = CreateMesh("assets/cube/cube.obj")) {
  //   mesh_id = *mesh_id_opt;
  // } else {
  //   std::cerr << "Failed to create mesh." << std::endl;
  //   throw InitException();
  // }
  // Mesh& cube_mesh = meshes_[mesh_id];

  // resource::Handle<resource::Image> image_handle = 
  //   resource_system_->LoadImage("assets/cube/default.png");
  // if (!image_handle.IsValid()) {
  //   std::cerr << "Failed to load image." << std::endl;
  //   throw InitException();
  // }
  // resource::Image& image = image_handle.Get();

  // command_buffer_.SetPlatform(gal_platform_.get());

  // gal::command::SetViewport set_viewport;
  // set_viewport.x = 0;
  // set_viewport.y = 0;
  // set_viewport.width = window_->GetWidth();
  // set_viewport.height = window_->GetHeight();
  // command_buffer_.Add(set_viewport);

  // gal::GALShader vert_shader;
  // if (!vert_shader.Create(gal_platform_.get(), gal::ShaderType::Vertex, kVertShaderSrc)) {
  //   std::cerr << "Failed to create GAL vertex shader." << std::endl;
  //   throw InitException();
  // }
   
  // gal::GALShader frag_shader;
  // if (!frag_shader.Create(gal_platform_.get(), gal::ShaderType::Fragment, kFragShaderSrc)) {
  //   std::cerr << "Failed to create GAL fragment shader." << std::endl;
  //   throw InitException();
  // }
  
  // gal::GALPipeline pipeline;
  // if (!pipeline.Create(gal_platform_.get(), vert_shader, frag_shader)) {
  //   std::cerr << "Failed to create GAL pipeline." << std::endl;
  //   throw InitException();
  // }
  // pipeline.GetVertexDesc().SetAttribute(0, 3);
  // pipeline.GetVertexDesc().SetAttribute(2, 2);

  // frag_shader.Destroy();
  // vert_shader.Destroy();

  // gal::command::SetPipeline set_pipeline;
  // set_pipeline.pipeline = pipeline;
  // command_buffer_.Add(set_pipeline);

  // struct {
  //   glm::mat4 model_mat;
  //   glm::mat4 view_mat;
  //   glm::mat4 proj_mat;
  // } uniform_data;

  // float aspect_ratio = 
  //   static_cast<float>(window_->GetWidth()) / static_cast<float>(window_->GetHeight());
  // uniform_data.model_mat = glm::mat4{1.f};
  // uniform_data.view_mat = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, -10.f}) *
  //     glm::rotate(glm::mat4{1.f}, glm::radians(-15.f), glm::vec3{0.f, 1.f, 0.f});
  // uniform_data.proj_mat = glm::perspective(glm::radians(30.f), aspect_ratio, 0.1f, 1000.f);

  // resource::HandleGALBuffer uniform_buf_handle = 
  //     resource_manager_->CreateBuffer(gal::BufferType::Uniform, 
  //                                     reinterpret_cast<uint8_t*>(&uniform_data),
  //                                     sizeof(uniform_data));
  // if (!uniform_buf_handle.IsValid()) {
  //   std::cerr << "Failed to create GAL buffer for uniforms." << std::endl;
  //   throw InitException();
  // }

  // gal::command::SetUniformBuffer set_uniform_buf;
  // set_uniform_buf.buffer = uniform_buf_handle.Get();
  // set_uniform_buf.idx = 0;
  // command_buffer_.Add(set_uniform_buf);
  
  // resource::HandleGALTexture texture_handle =
  //     resource_manager_->CreateTexture(gal::TextureType::Texture2D, gal::TextureFormat::RGB,
  //                                      image.width, image.height, image.pixels.data());
  // if (!texture_handle.IsValid()) {
  //   std::cerr << "Failed to create GAL texture." << std::endl;
  //   throw InitException();
  // }

  // gal::GALTextureSampler tex_sampler;
  // if (!tex_sampler.Create(gal_platform_.get(), texture_handle.Get())) {
  //   std::cerr << "Failed to create GAL texture sampler." << std::endl;
  //   throw InitException();
  // }

  // gal::command::SetTextureSampler set_tex_sampler;
  // set_tex_sampler.sampler = tex_sampler;
  // set_tex_sampler.idx = 1;
  // command_buffer_.Add(set_tex_sampler);           

  // gal::command::SetVertexBuffer set_pos_vert_buf;
  // set_pos_vert_buf.buffer = cube_mesh.pos_buf_.Get();
  // set_pos_vert_buf.vert_idx = 0;
  // command_buffer_.Add(set_pos_vert_buf);

  // gal::command::SetVertexBuffer set_texcoord_vert_buf;
  // set_texcoord_vert_buf.buffer = cube_mesh.texcoord_buf_.Get();
  // set_texcoord_vert_buf.vert_idx = 2;
  // command_buffer_.Add(set_texcoord_vert_buf);
  
  // gal::command::ClearScreen clear_screen;
  // clear_screen.color = glm::vec4{0.f, 0.f, 0.f, 1.f};
  // clear_screen.clear_color = true;
  // clear_screen.clear_depth = true;
  // command_buffer_.Add(clear_screen);

  // gal::command::DrawTriangles draw_triangles;
  // draw_triangles.num_triangles = cube_mesh.faces;
  // command_buffer_.Add(draw_triangles);

  // resource::ModelLoader model_loader;
  // std::shared_ptr<resource::Model> model = model_loader.LoadModel("assets/cube/cube.obj");
}

Renderer::~Renderer() {
  command_buffer_.Destroy();

  pipeline_.Destroy();

  // resource_manager_.release();
  gal_platform_.release();

  resource_system_ = nullptr;
  window_ = nullptr;
}

void Renderer::Tick() {
  gal_platform_->StartTick();

  gal_platform_->ExecuteCommandBuffer(command_buffer_);

  gal_platform_->EndTick();
  // gal_platform_->ExecuteCommandBuffer(command_buffer_);
}

// std::optional<MeshId> Renderer::CreateMesh(const std::string& file_path) {
//   resource::Handle<resource::Model> model_handle = resource_system_->LoadModel(file_path);
//   if (!model_handle.IsValid()) {
//     return std::nullopt;
//   }
//   resource::Model& model = model_handle.Get();

//   static int counter = 0;
//   ++counter;

//   MeshId mesh_id = counter;
//   Mesh& mesh = meshes_[mesh_id];

//   if (!model.positions.empty()) {
//     resource::HandleGALBuffer pos_buf_handle = 
//         resource_manager_->CreateBuffer(gal::BufferType::Vertex, 
//                                         reinterpret_cast<uint8_t*>(model.positions.data()),
//                                         model.positions.size() * sizeof(glm::vec3));
//     if (!pos_buf_handle.IsValid()) {
//       return std::nullopt;
//     }
//     mesh.pos_buf_ = std::move(pos_buf_handle);
//   }

//   // TODO(colintan): Add for normals data

//   if (!model.texcoords.empty()) {
//     resource::HandleGALBuffer texcoord_buf_handle = 
//         resource_manager_->CreateBuffer(gal::BufferType::Vertex, 
//                                         reinterpret_cast<uint8_t*>(model.texcoords.data()),
//                                         model.texcoords.size() * sizeof(glm::vec2));
//     if (!texcoord_buf_handle.IsValid()) {
//       return std::nullopt;
//     }
//     mesh.texcoord_buf_ = std::move(texcoord_buf_handle);
//   }

//   mesh.faces = model.faces;

//   // TODO(colintan): Release the resource - automatically or via a call to ResourceManager

//   return mesh_id;
// }

} // namespace