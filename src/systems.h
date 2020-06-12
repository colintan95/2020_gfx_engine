#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <memory>
#include "platform/window.h"
#include "platform/input_manager.h"

class Systems {
public:
  bool InitSystems();
  void DestroySystems();

  platform::Window* GetWindow() { return window_.get(); }

private:
  std::unique_ptr<platform::Window> window_;
  std::unique_ptr<platform::InputManager> input_manager_;
};

#endif // SYSTEMS_H_