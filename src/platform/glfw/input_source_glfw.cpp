#include "platform/glfw/input_source_glfw.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace input {
namespace internal {

bool InputSourceGLFW::Initialize() {
  return true;
}

void InputSourceGLFW::Cleanup() {

}

void InputSourceGLFW::Tick() {
  glfwPollEvents();
}

} // namespace
} // namespace