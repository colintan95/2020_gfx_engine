#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "gal/gal.h"
#include "resource/image_loader.h"
#include "resource/model_loader.h"
#include "platform/window.h"

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

constexpr float kAspectRatio = 
    static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight);

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

Application::Application(window::Window* window) {
  window_ = window;
  window_->CreateWindow(kScreenWidth, kScreenHeight, "Hello World");

  resource::ModelLoader model_loader;
  std::shared_ptr<resource::Model> model = model_loader.LoadModel("assets/cube/cube.obj");
  if (model == nullptr) {
    std::cerr << "Failed to load model." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  resource::ImageLoader image_loader;
  std::shared_ptr<resource::Image> image = image_loader.LoadImage("assets/cube/default.png");
  if (image == nullptr) {
    std::cerr << "Failed to load image." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  gal::command::SetViewport set_viewport;
  set_viewport.x = 0;
  set_viewport.y = 0;
  set_viewport.width = kScreenWidth;
  set_viewport.height = kScreenHeight;
  command_buffer_.Add(set_viewport);

  auto vert_shader_opt = gal::GALShader::Create(gal::ShaderType::Vertex, kVertShaderSrc);
  if (!vert_shader_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex shader." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  auto frag_shader_opt = gal::GALShader::Create(gal::ShaderType::Fragment, kFragShaderSrc);
  if (!frag_shader_opt.has_value()) {
    std::cerr << "Failed to create GAL fragment shader." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  auto pipeline_opt = gal::GALPipeline::Create(*vert_shader_opt, *frag_shader_opt);
  if (!pipeline_opt.has_value()) {
    std::cerr << "Failed to create GAL pipeline." << std::endl;
    std::exit(EXIT_FAILURE);
  }          

  gal::command::SetPipeline set_pipeline;
  set_pipeline.pipeline = *pipeline_opt;
  command_buffer_.Add(set_pipeline);

  struct {
    glm::mat4 model_mat;
    glm::mat4 view_mat;
    glm::mat4 proj_mat;
  } uniform_data;

  uniform_data.model_mat = glm::mat4{1.f};
  // uniform_data.view_mat = glm::mat4{1.f};
  uniform_data.view_mat = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, -10.f}) *
      glm::rotate(glm::mat4{1.f}, glm::radians(-15.f), glm::vec3{0.f, 1.f, 0.f});
  uniform_data.proj_mat = glm::perspective(glm::radians(30.f), kAspectRatio, 0.1f, 1000.f);

  auto uniform_buf_opt = gal::GALBuffer::Create(gal::BufferType::Uniform, 
                                                reinterpret_cast<uint8_t*>(&uniform_data), 
                                                sizeof(uniform_data));
  if (!uniform_buf_opt.has_value()) {
    std::cerr << "Failed to create GAL buffer for uniforms." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  gal::command::SetUniformBuffer set_uniform_buf;
  set_uniform_buf.buffer = *uniform_buf_opt;
  set_uniform_buf.idx = 0;
  command_buffer_.Add(set_uniform_buf);

  auto texture_opt = gal::GALTexture::Create(gal::TextureType::Texture2D, gal::TextureFormat::RGB,
                                             image->width, image->height, image->pixels.data());
  if (!texture_opt.has_value()) {
    std::cerr << "Failed to create GAL texture." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  auto tex_sampler_opt = gal::GALTextureSampler::Create(*texture_opt);
  if (!tex_sampler_opt.has_value()) {
    std::cerr << "Failed to create GAL texture sampler." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  gal::command::SetTextureSampler set_tex_sampler;
  set_tex_sampler.sampler = *tex_sampler_opt;
  set_tex_sampler.idx = 1;
  command_buffer_.Add(set_tex_sampler);

  // uniform_data.view_mat = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, -10.f}) *
  //     glm::rotate(glm::mat4{1.f}, glm::radians(-15.f), glm::vec3{0.f, 1.f, 0.f});
  // uniform_buf_opt->Update(reinterpret_cast<uint8_t*>(&uniform_data), 0, sizeof(uniform_data));           

  auto vert_desc_opt = gal::GALVertexDesc::Create();
  if (!vert_desc_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex description." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  vert_desc_opt->entries = std::make_shared<std::vector<gal::GALVertexDesc::Entry>>();
  auto& entries = *(vert_desc_opt->entries);
  entries.push_back({});
  entries[0].index = 0;
  entries[0].size = 3;
  entries.push_back({});
  entries[1].index = 2;
  entries[1].size = 2;

  gal::command::SetVertexDesc set_vert_desc;
  set_vert_desc.vert_desc = *vert_desc_opt;
  command_buffer_.Add(set_vert_desc);

  auto pos_vert_buf_opt = gal::GALVertexBuffer::Create(
      reinterpret_cast<uint8_t*>(model->positions.data()), 
      model->positions.size() * sizeof(glm::vec3)
  );
  if (!pos_vert_buf_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex buffer for positions." << std::endl;
    std::exit(EXIT_FAILURE);
  }        

  gal::command::SetVertexBuffer set_pos_vert_buf;
  set_pos_vert_buf.buffer = *pos_vert_buf_opt;
  set_pos_vert_buf.vert_idx = 0;
  command_buffer_.Add(set_pos_vert_buf);

  auto texcoord_vert_buf_opt = gal::GALVertexBuffer::Create(
      reinterpret_cast<uint8_t*>(model->texcoords.data()),
      model->texcoords.size() * sizeof(glm::vec2)
  );    
  if (!texcoord_vert_buf_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex buffer for texcoords." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  gal::command::SetVertexBuffer set_texcoord_vert_buf;
  set_texcoord_vert_buf.buffer = *texcoord_vert_buf_opt;
  set_texcoord_vert_buf.vert_idx = 2;
  command_buffer_.Add(set_texcoord_vert_buf);
  
  gal::command::ClearScreen clear_screen;
  clear_screen.color = glm::vec4{0.f, 0.f, 0.f, 1.f};
  clear_screen.clear_color = true;
  clear_screen.clear_depth = true;
  command_buffer_.Add(clear_screen);

  gal::command::DrawTriangles draw_triangles;
  draw_triangles.num_triangles = model->faces;
  command_buffer_.Add(draw_triangles);
}

Application::~Application() {}

void Application::RunLoop() {
  gal::ExecuteCommandBuffer(command_buffer_);
  while (!window_->ShouldClose()) {

    gal::ExecuteCommandBuffer(command_buffer_);
   
    window_->Tick();
  }
}