#include "application.h"
#include "platform/platform.h"

#include "window/window_manager.h"
#include "window/window.h"

void Test() {
  window::WindowManager window_manager;

  window_manager.Initialize();

  std::optional<window::WindowRef> window_ref_opt = 
      window_manager.CreateWindow(1920, 1080, "Hello World");
  if (!window_ref_opt.has_value()) {
    return;
  }

  window_manager.Cleanup();
}

int main() {
  Test();

  // platform::Platform platform;
  // platform.Initialize();

  // std::unique_ptr<Application> app = std::make_unique<Application>(&platform);
  // app->RunLoop();
  // app.reset();

  // platform.Cleanup();
  
  return 0;
}