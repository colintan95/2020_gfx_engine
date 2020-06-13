#include "window/glfw/window_manager_glfw.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

namespace window {

bool WindowManagerImplGLFW::Initialize() {
  if (!glfwInit()) {
    std::cerr << "Failed glfwInit()" << std::endl;
    return false;
  }
  return true;
}

void WindowManagerImplGLFW::Cleanup() {

}

std::unique_ptr<WindowManagerImpl> WindowManagerImpl::Create() {
  return std::make_unique<WindowManagerImplGLFW>();
}

} // namespace