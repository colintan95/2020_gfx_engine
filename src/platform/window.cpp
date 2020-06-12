#include "platform/window.h"

namespace platform {

Window::Window()  {
  impl_ = CreateWindowImpl();
}

Window::~Window() {
  impl_.reset();
}

} // platform