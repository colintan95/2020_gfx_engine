#include "platform/window.h"

#include "platform/impl_creator.h"

namespace window {

Window::Window(platform::internal::ImplCreator* impl_creator)  {
  impl_ = impl_creator->CreateWindowImpl();
}

Window::~Window() {
  impl_.reset();
}

} // platform