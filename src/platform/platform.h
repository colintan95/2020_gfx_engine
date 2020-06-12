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

  platform::Window* GetWindow() { return window_.get(); }

private:
  std::unique_ptr<PlatformImpl> impl_;
  std::unique_ptr<Window> window_;
  std::unique_ptr<InputManager> input_manager_;
};

// Defined in platform-specific code
std::unique_ptr<PlatformImpl> CreatePlatformImpl();

} // namespace

#endif