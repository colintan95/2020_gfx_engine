#include "application.h"
#include "platform/platform.h"

#include <iostream>
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

  while(1) {
    window_manager.Tick();

    while (std::optional<window::Event> event_opt = window_ref_opt->ConsumeEvent()) {
      if (event_opt->IsType<window::KeyboardEvent>()) {
        window::KeyboardEvent event = event_opt->AsType<window::KeyboardEvent>();

        if (event.key == window::KeyInput::KeyA) {
          if (event.action == window::KeyAction::Down) {
            std::cout << "Key A Down" << std::endl;
          } else {
            std::cout << "Key A Up" << std::endl;
          }
        }
      }
    }

    window_ref_opt->SwapBuffers();
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