#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <memory>
#include "window/window.h"
#include "input/input_manager.h"

class Systems {
public:
  bool InitSystems();
  void DestroySystems();

  window::Window* GetWindow() { return window_.get(); }

private:
  std::unique_ptr<window::Window> window_;
  std::unique_ptr<input::InputManager> input_manager_;
};

#endif // SYSTEMS_H_