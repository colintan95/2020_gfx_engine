#ifndef PLATFORM_GLFW_PLATFORM_GLFW_H_
#define PLATFORM_GLFW_PLATFORM_GLFW_H_

#include "platform/platform.h"

namespace platform {

class PlatformImplGLFW : public PlatformImpl {
public:
  bool Initialize() final;
  void Cleanup() final;
};

} // namespace

#endif // PLATFORM_GLFW_PLATFORM_GLFW_H_