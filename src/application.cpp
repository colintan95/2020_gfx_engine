#include "application.h"

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
#include "resource/image_loader.h"
#include "resource/model_loader.h"

#include "resource/gal_resource.h"
#include "resource/gal_resource_manager.h"

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

bool Application::Initialize(window::Window* window) {
  window_= window;

  

  return true;
}

// TODO(colintan): Do proper cleanup if needed
void Application::Cleanup() {
  resource_manager_.release();
  gal_platform_.Cleanup();
  window_ = nullptr;
}

void Application::Tick() {
  gal_platform_.ExecuteCommandBuffer(command_buffer_);
}