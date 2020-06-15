#include "window/glfw/window_manager_glfw.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <new>
#include "window/window_manager.h"

namespace window {

WindowManagerImplGLFW::WindowManagerImplGLFW() {
  if (!glfwInit()) {
    std::cerr << "Failed glfwInit()" << std::endl;
    throw WindowManager::InitException();
  }
}

std::unique_ptr<WindowManagerImpl> WindowManagerImpl::Create() {
  try {
    return std::make_unique<WindowManagerImplGLFW>();
  } catch (std::bad_alloc& ba) {
    throw WindowManager::InitException();
  }
}

} // namespace