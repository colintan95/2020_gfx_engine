#include "platform/window.h"

namespace window {

Window::Window()  {
  impl_ = CreateWindowImpl();
}

Window::~Window() {
  impl_.reset();
}

} // platform