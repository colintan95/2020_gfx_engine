#include "platform/glfw/platform_glfw.h"

#include <memory>
#include "platform/platform.h"

namespace platform {

bool PlatformImplGLFW::Initialize() {
  return true;
}

void PlatformImplGLFW::Cleanup() {

}

std::unique_ptr<PlatformImpl> CreatePlatformImpl() {
  return std::make_unique<PlatformImplGLFW>();
}

} // namespace