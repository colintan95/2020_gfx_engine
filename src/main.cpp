#include <iostream>
#include <memory>
#include "event/event_manager.h"
#include "render/renderer.h"
#include "window/window.h"
#include "window/window_manager.h"

int main() {
  window::WindowManager window_manager;
  if (!window_manager.Initialize()) {
    std::cerr << "Failed to initialize window manager." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  window::Window* window = window_manager.CreateWindow(1920, 1080, "Hello World");
  if (window == nullptr) {
    std::cerr << "Failed to create window." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  
  event::EventManager event_manager;
  if (!event_manager.Initialize(window)) {
    std::cerr << "Failed to initialize event manager." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<render::Renderer> renderer = std::make_unique<render::Renderer>();
  if (!renderer->Initialize(window)) {
    std::cerr << "Failed to initialize renderer." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  while (!window_manager.ShouldClose()) {
    window_manager.Tick();
    event_manager.Tick();
    renderer->Tick();

    window->SwapBuffers();
  }

  renderer->Cleanup();
  renderer.release();

  event_manager.Cleanup();

  window_manager.DestroyWindow(window);

  window_manager.Cleanup();
  
  return 0;
}