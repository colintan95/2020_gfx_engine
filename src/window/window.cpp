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

std::optional<event::Event> Window::ConsumeEvent() {
  return impl_->ConsumeEvent();
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
}

void Window::AddEventConsumer(EventConsumer* consumer) {

}

void Window::RemoveEventConsumer(EventConsumer* consumer) {

}

} // namespace