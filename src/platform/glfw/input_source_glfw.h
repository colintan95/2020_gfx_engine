#ifndef PLATFORM_GLFW_INPUT_SOURCE_GLFW_H_
#define PLATFORM_GLFW_INPUT_SOURCE_GLFW_H_

#include "platform/input_source.h"

namespace input {

class InputManager;

namespace internal {

class InputSourceGLFW : public InputSource {
public:
  bool Initialize() final;
  void Cleanup() final;

  void Tick() final;
};

} // namespace
} // namespace

#endif // PLATFORM_GLFW_INPUT_SOURCE_GLFW_H_