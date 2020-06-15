#include <iostream>

#include <memory>
#include "event/event_manager.h"
#include "render/renderer.h"
#include "resource/resource_system.h"
#include "scene/scene.h"
#include "window/window.h"
#include "window/window_manager.h"

int main() {
  std::unique_ptr<resource::ResourceSystem> resource_system;
  
  try {
    resource_system = std::make_unique<resource::ResourceSystem>();
  } catch (resource::ResourceSystem::InitException& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<window::WindowManager> window_manager;

  try {
    window_manager = std::make_unique<window::WindowManager>();
  } catch (window::WindowManager::InitException& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  window::Window* window = window_manager->CreateWindow(1920, 1080, "Hello World");
  if (window == nullptr) {
    std::cerr << "Failed to create window." << std::endl;
    std::exit(EXIT_FAILURE);
  }
  
  std::unique_ptr<event::EventManager> event_manager;
  
  try {
    event_manager = std::make_unique<event::EventManager>(window);
  } catch (event::EventManager::InitException& e) {
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<render::Renderer> renderer = std::make_unique<render::Renderer>();
  if (!renderer->Initialize(window, resource_system.get())) {
    std::cerr << "Failed to initialize renderer." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::unique_ptr<scene::Scene> scene = std::make_unique<scene::Scene>();
  if (!scene->Initialize(renderer.get())) {
    std::cerr << "Failed to initialize scene." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  while (!window_manager->ShouldClose()) {
    window_manager->Tick();
    event_manager->Tick();

    scene->Tick();
    renderer->Tick();
    
    window->SwapBuffers();
  }

  scene->Cleanup();
  scene.release();

  renderer->Cleanup();
  renderer.release();

  event_manager.release();

  window_manager->DestroyWindow(window);
  window_manager.release();

  resource_system.release();
  
  return 0;
}