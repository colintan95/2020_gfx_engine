#include "platform/window.h"

namespace platform {

Window::Window(InputManager* input_manager)  {
  impl_ = CreateWindowImpl();
  input_manager_ = input_manager;
}

Window::~Window() {
  input_manager_ = nullptr;
  impl_.reset();
}

} // platform