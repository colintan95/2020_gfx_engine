#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

constexpr int kScreenWidth = 1920;
constexpr int kScreenHeight = 1080;

int main() {
  glfwSetErrorCallback([](int err_code, const char* desc) {
    std::cerr << "Error Code " << err_code << ": " << desc << std::endl;
  });

  if (!glfwInit()) {
    std::exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  GLFWwindow* window = glfwCreateWindow(kScreenWidth, kScreenHeight, "Hello World",  nullptr, 
                                        nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    std::exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glViewport(0, 0, kScreenWidth, kScreenHeight);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}