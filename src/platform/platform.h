#ifndef PLATFORM_PLATFORM_H_
#define PLATFORM_PLATFORM_H_

#include <memory>
#include "platform/window.h"
#include "platform/input_manager.h"

namespace platform {

namespace internal {
class ImplCreator;
} // namespace

class PlatformImpl {
public:
  virtual ~PlatformImpl() {}

  virtual bool Initialize() = 0;
  virtual void Cleanup() = 0;

  virtual void Tick() = 0;
};

class Platform {
public:
  Platform();
  ~Platform();

  bool Initialize();
  void Cleanup();

  // TODO(colintan): Figure out where this should be called - window's Tick() should be called
  // separately - make it more clear that it doesn't get called here
  void Tick();

  input::InputManager* GetInputManager() { return input_manager_.get(); }
  window::Window* GetWindow() { return window_.get(); }

private:
  std::unique_ptr<internal::ImplCreator> impl_creator_;
  std::unique_ptr<PlatformImpl> impl_;
  std::unique_ptr<window::Window> window_;
  std::unique_ptr<input::InputManager> input_manager_;
};

} // namespace

#endif