#include "application.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "gal/gal.h"
#include "resource/image_loader.h"
#include "resource/model_loader.h"
#include "window/window.h"

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

constexpr float kAspectRatio = 
    static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight);

const char kVertShaderSrc[] =
    "#version 330 core\n"
    "layout(location = 0) in vec3 vert_pos;\n"
    "void main() {\n"
    "  gl_Position = vec4(vert_pos, 1.0);\n"
    "}";
const char kFragShaderSrc[] =
    "#version 330 core\n"
    "out vec4 out_color;\n"
    "void main() {\n"
    "  out_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}";

std::vector<glm::vec3> kVertices{
    {-0.5f, -0.5f, 0.f}, {0.f, 0.5f, 0.f}, {0.5f, -0.5f, 0.f}
  };

Application::Application() {
  try {
    window_ = std::make_unique<window::Window>(kScreenWidth, kScreenHeight, "Hello World");
  } catch (window::WindowInitException& e) {
    // TODO(colintan): Do something else
    std::exit(EXIT_FAILURE);
  } 

  // glEnable(GL_TEXTURE_2D);
  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);

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

  auto pos_vert_buf_opt = gal::GALVertexBuffer::Create(reinterpret_cast<uint8_t*>(kVertices.data()),
                                                       kVertices.size() * sizeof(glm::vec3));
  if (!pos_vert_buf_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex buffer." << std::endl;
    std::exit(EXIT_FAILURE);
  }                                                 

  auto vert_desc_opt = gal::GALVertexDesc::Create();
  if (!vert_desc_opt.has_value()) {
    std::cerr << "Failed to create GAL vertex description." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  vert_desc_opt->Index(0).buffer = *pos_vert_buf_opt;
  vert_desc_opt->Index(0).index = 0;
  vert_desc_opt->Index(0).size = 3;

  gal::command::SetViewport set_viewport;
  set_viewport.x = 0;
  set_viewport.y = 0;
  set_viewport.width = kScreenWidth;
  set_viewport.height = kScreenHeight;
  command_buffer_.Add(set_viewport);

  gal::command::SetPipeline set_pipeline;
  set_pipeline.pipeline = *pipeline_opt;
  command_buffer_.Add(set_pipeline);

  gal::command::SetVertexBuffer set_vert_buf;
  set_vert_buf.buffer = *pos_vert_buf_opt;
  command_buffer_.Add(set_vert_buf);

  gal::command::SetVertexDesc set_vert_desc;
  set_vert_desc.vert_desc = *vert_desc_opt;
  command_buffer_.Add(set_vert_desc);

  gal::command::ClearScreen clear_screen;
  clear_screen.color = glm::vec4{0.f, 0.f, 0.f, 1.f};
  clear_screen.clear_color = true;
  command_buffer_.Add(clear_screen);

  gal::command::DrawTriangles draw_triangles;
  draw_triangles.num_triangles = 1;
  command_buffer_.Add(draw_triangles);
}

Application::~Application() {
  window_.reset();
}

void Application::RunLoop() {
  gal::ExecuteCommandBuffer(command_buffer_);
  while (!window_->ShouldClose()) {

    gal::ExecuteCommandBuffer(command_buffer_);
   
    window_->Tick();
  }
}