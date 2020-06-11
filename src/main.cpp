#include <iostream>
#include <cstdlib>
#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cassert>
#include <memory>
#include "resource/image_loader.h"
#include "resource/model_loader.h"

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

constexpr float kAspectRatio = 
    static_cast<float>(kScreenWidth) / static_cast<float>(kScreenHeight);

const char kVertShaderSrc[] =
    "#version 330 core\n"
    "layout(location = 0) in vec3 vert_pos;\n"
    "layout(location = 2) in vec2 vert_texcoord;\n"
    "out vec2 frag_texcoord;\n"
    "uniform mat4 mvp_mat;\n"
    "void main() {\n"
    "  frag_texcoord = vert_texcoord;\n"
    "  gl_Position = mvp_mat * vec4(vert_pos, 1.0);\n"
    "}";
const char kFragShaderSrc[] =
    "#version 330 core\n"
    "in vec2 frag_texcoord;\n"
    "out vec4 out_color;\n"
    "uniform sampler2D tex_sampler;\n"
    "void main() {\n"
    "  out_color = texture(tex_sampler, frag_texcoord);\n"
    "}";

bool CheckShaderSuccess(int shader) {
  int gl_success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(shader, 256, nullptr, log);

    std::cerr << "Failed to compile shader." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

bool CheckProgramSuccess(int program) {
  int gl_success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &gl_success);

  if (gl_success == GL_TRUE) {
    return true;
  } else {
    char log[256];
    glGetShaderInfoLog(program, 256, nullptr, log);

    std::cerr << "Failed to link program." << std::endl;
    std::cerr << log << std::endl;

    return false;
  }
}

int main() {
  glfwSetErrorCallback([](int err_code, const char* desc) {
    std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  });

  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Hello World",  nullptr, 
                                        nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    return false;
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

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

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  assert(image->format == resource::ImageFormat::RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               image->pixels.data());

  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vert_shader_src = static_cast<const char*>(kVertShaderSrc);
  glShaderSource(vert_shader, 1, &vert_shader_src, nullptr);
  glCompileShader(vert_shader);
  if (!CheckShaderSuccess(vert_shader)) {
    std::exit(EXIT_FAILURE);
  }

  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* frag_shader_src = static_cast<const char*>(kFragShaderSrc);
  glShaderSource(frag_shader, 1, &frag_shader_src, nullptr);
  glCompileShader(frag_shader);
  if (!CheckShaderSuccess(frag_shader)) {
    std::exit(EXIT_FAILURE);
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);
  glLinkProgram(program);
  if (!CheckProgramSuccess(program)) {
    std::exit(EXIT_FAILURE);
  }
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  glUseProgram(program);

  glm::mat4 model_mat{1.f};
  glm::mat4 view_mat = 
      glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, -10.f}) *
      glm::rotate(glm::mat4{1.f}, glm::radians(-15.f), glm::vec3{0.f, 1.f, 0.f});

  glm::mat4 proj_mat = glm::perspective(glm::radians(30.f), kAspectRatio, 0.1f, 1000.f);
  glm::mat4 mvp_mat = proj_mat * view_mat * model_mat;

  GLint mvp_mat_loc = glGetUniformLocation(program, "mvp_mat");
  glUniformMatrix4fv(mvp_mat_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture);

  GLint texture_loc = glGetUniformLocation(program, "tex_sampler");
  glUniform1i(texture_loc, 1);

  GLuint vao;
  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint pos_vbo;
  glCreateBuffers(1, &pos_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
  glBufferData(GL_ARRAY_BUFFER, model->positions.size() * sizeof(glm::vec3), 
               glm::value_ptr(model->positions[0]), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
  glEnableVertexAttribArray(0);

  GLuint texcoord_vbo;
  glCreateBuffers(1, &texcoord_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_vbo);
  glBufferData(GL_ARRAY_BUFFER, model->texcoords.size() * sizeof(glm::vec3), 
               glm::value_ptr(model->texcoords[0]), GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glViewport(0, 0, kScreenWidth, kScreenHeight);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, model->faces * 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteTextures(1, &texture);
  glDeleteBuffers(1, &texcoord_vbo);
  glDeleteBuffers(1, &pos_vbo);
  glDeleteVertexArrays(1, &vao);
  glDeleteProgram(program);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}