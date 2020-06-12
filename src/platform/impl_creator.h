#ifndef PLATFORM_IMPL_CREATOR_H_
#define PLATFORM_IMPL_CREATOR_H_

#include <memory>

namespace input {
class InputManager;

namespace internal {
class InputSource;
} // namespace

} // namespace

namespace window {
class WindowImpl;
} // namespace

namespace platform {

class PlatformImpl;

namespace internal {

// Used to create the platform-specific impls, so that we can share state among them
//
// TODO(colintan): Rename to ImplManager
class ImplCreator {
public:
  virtual ~ImplCreator() {}

  virtual std::unique_ptr<PlatformImpl> CreatePlatformImpl() = 0;
  
  virtual std::unique_ptr<input::internal::InputSource> CreateInputSource() = 0;

  virtual std::unique_ptr<window::WindowImpl> CreateWindowImpl() = 0;
};

std::unique_ptr<ImplCreator> CreateImplCreator();

} // namespace
} // namespace

#endif // PLATFORM_IMPL_CREATOR_H_