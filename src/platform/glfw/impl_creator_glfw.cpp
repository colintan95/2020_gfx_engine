#include "platform/glfw/impl_creator_glfw.h"

#include <memory>
#include "platform/glfw/input_source_glfw.h"
#include "platform/glfw/platform_glfw.h"
#include "platform/glfw/window_glfw.h"

namespace platform {
namespace internal {

std::unique_ptr<PlatformImpl> ImplCreatorGLFW::CreatePlatformImpl() {
  return std::make_unique<PlatformImplGLFW>();
}

std::unique_ptr<input::internal::InputSource> ImplCreatorGLFW::CreateInputSource() {
  return std::make_unique<input::internal::InputSourceGLFW>();
}

std::unique_ptr<window::WindowImpl> ImplCreatorGLFW::CreateWindowImpl() {
  return std::make_unique<window::WindowImplGLFW>();
}

std::unique_ptr<ImplCreator> CreateImplCreator() {
  return std::make_unique<ImplCreatorGLFW>();
}

} // namespace
} // namespace