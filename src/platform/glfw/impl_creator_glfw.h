#ifndef PLATFORM_GLFW_IMPL_CREATOR_GLFW_H_
#define PATLFORM_GLFW_IMPL_CREATOR_GLFW_H_

#include "platform/impl_creator.h"

namespace platform {
namespace internal {

class ImplCreatorGLFW : public ImplCreator {
public:
  virtual std::unique_ptr<PlatformImpl> CreatePlatformImpl() final;
  
  virtual std::unique_ptr<input::internal::InputSource> CreateInputSource() final;

  std::unique_ptr<window::WindowImpl> CreateWindowImpl() final;
};

} // namespace
} // namespace

#endif // PATLFORM_GLFW_IMPL_CREATOR_GLFW_H_