#include "application.h"

#include <iostream>
#include <memory>
#include "event/event_manager.h"
#include "window/event_consumer.h"
#include "window/window_manager.h"
#include "window/window.h"

class MyEventHandlerImpl : public event::IEventHandlerImpl {
public:
  void Handle(const event::Event& event) {
    std::cout << "Event!" << std::endl;
  }
};

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

  std::unique_ptr<event::EventHandler> event_handler = 
      event_manager.CreateHandler<MyEventHandlerImpl>();

  Application application;
  if (!application.Initialize(window)) {
    std::cerr << "Failed to initialize application." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  while (!window_manager.ShouldClose()) {
    window_manager.Tick();
    event_manager.Tick();
    application.Tick();

    window->SwapBuffers();
  }

  event_handler.release();
  event_manager.Cleanup();

  window_manager.DestroyWindow(window);

  application.Cleanup();
  window_manager.Cleanup();
  
  return 0;
}