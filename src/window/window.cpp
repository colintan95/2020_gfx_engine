#include "window/window.h"

#include "window/event_consumer.h"
#include "window/window_impl.h"

namespace window {

Window::Window() { 
  impl_ = internal::WindowImpl::Create();
}

void Window::SwapBuffers() {
  impl_->SwapBuffers();
}

bool Window::ShouldClose() {
  return impl_->ShouldClose();
}

bool Window::CreateWindow(int width, int height, const std::string& title) {
  return impl_->CreateWindow(width, height, title);
}

void Window::DestroyWindow() {
  impl_->DestroyWindow();
}

void Window::Tick() {
  impl_->Tick();

  while (std::optional<event::Event> event_opt = impl_->ConsumeEvent()) {
    for (EventConsumer* consumer : event_consumers_) {
      consumer->EnqueueEvent(*event_opt);
    }
  }
}

void Window::AddEventConsumer(EventConsumer* consumer) {
  event_consumers_.insert(consumer);
}

void Window::RemoveEventConsumer(EventConsumer* consumer) {
  event_consumers_.erase(consumer);
}

} // namespace