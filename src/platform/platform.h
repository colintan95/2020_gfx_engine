#ifndef PLATFORM_PLATFORM_H_
#define PLATFORM_PLATFORM_H_

#include <memory>
#include "platform/window.h"
#include "platform/input_manager.h"

namespace platform {

class PlatformImpl {
public:
  virtual ~PlatformImpl() {}

  virtual bool Initialize() = 0;
  virtual void Cleanup() = 0;
};

class Platform {
public:
  Platform();
  ~Platform();

  bool Initialize();
  void Cleanup();

  window::Window* GetWindow() { return window_.get(); }

private:
  std::unique_ptr<PlatformImpl> impl_;
  std::unique_ptr<window::Window> window_;
  std::unique_ptr<input::InputManager> input_manager_;
};

// Defined in platform-specific code
std::unique_ptr<PlatformImpl> CreatePlatformImpl();

} // namespace

#endif