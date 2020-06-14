#include "window/window.h"

#include "window/event_consumer.h"
#include "window/window_internal.h"

namespace window {

WindowRef::WindowRef(WindowId window_id, internal::Window* impl) { 
  window_id_ = window_id; 
  impl_ = impl; 
}

void WindowRef::SwapBuffers() {
  impl_->SwapBuffers();
}

std::optional<event::Event> WindowRef::ConsumeEvent() {
  return impl_->ConsumeEvent();
}

bool WindowRef::ShouldClose() {
  return impl_->ShouldClose();
}

void WindowRef::AddEventConsumer(EventConsumer* consumer) {

}

void WindowRef::RemoveEventConsumer(EventConsumer* consumer) {

}

} // namespace