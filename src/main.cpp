#include <iostream>
#include <cstdlib>
#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include "resource/model_loader.h"

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

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

  resource::ModelLoader model_loader;
  std::shared_ptr<resource::Model> model = model_loader.LoadModel("assets/cube/cube.obj");

  glViewport(0, 0, kScreenWidth, kScreenHeight);

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

  GLuint vao;
  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glm::vec3 vertices[] = {
    {-0.5f, -0.5f, 0.f}, {0.f, 0.5f, 0.f}, {0.5f, -0.5f, 0.f}
  };

  GLuint vbo;
  glCreateBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glDeleteProgram(program);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}