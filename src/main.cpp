#include "application.h"

#include <iostream>
#include "event/event_manager.h"
#include "window/window_manager.h"
#include "window/window.h"

int main() {
  window::WindowManager window_manager;
  if (!window_manager.Initialize()) {
    std::cerr << "Failed to initialize window manager." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::optional<window::WindowRef> window_ref_opt = 
      window_manager.CreateWindow(1920, 1080, "Hello World");
  if (!window_ref_opt) {
    std::cerr << "Failed to create window." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  event::EventManager event_manager(&(*window_ref_opt));

  Application application;
  if (!application.Initialize(*window_ref_opt)) {
    std::cerr << "Failed to initialize application." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  while (!window_manager.ShouldClose()) {
    window_manager.Tick();
    application.Tick();

    window_ref_opt->SwapBuffers();
  }

  application.Cleanup();
  window_manager.Cleanup();
  
  return 0;
}