#include "platform/glfw/platform_glfw.h"

#include <GLFW/glfw3.h>

#include <memory>
#include "platform/platform.h"

namespace platform {

bool PlatformImplGLFW::Initialize() {
  if (!glfwInit()) {
    return false;
  }
  initialized_ = true;
  return true;
}

void PlatformImplGLFW::Cleanup() {
  if (initialized_) {
    glfwTerminate();
  }
}

std::unique_ptr<PlatformImpl> CreatePlatformImpl() {
  return std::make_unique<PlatformImplGLFW>();
}

} // namespace