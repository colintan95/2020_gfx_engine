#ifndef PLATFORM_GLFW_PLATFORM_GLFW_H_
#define PLATFORM_GLFW_PLATFORM_GLFW_H_

#include "platform/platform.h"

namespace platform {

class PlatformImplGLFW : public PlatformImpl {
public:
  bool Initialize() final;
  void Cleanup() final;

  void Tick() final;

private:
  bool initialized_ = false;
};

} // namespace

#endif // PLATFORM_GLFW_PLATFORM_GLFW_H_