#include "window/window_manager.h"

#include <memory>
#include "window/window_internal.h"

namespace window {

WindowManager::WindowManager() {
  impl_ = WindowManagerImpl::Create();
}

WindowManager::~WindowManager() {
  impl_.reset();
}

bool WindowManager::Initialize() {
  return impl_->Initialize();
}

void WindowManager::Cleanup() {
  if (window_ != nullptr) {
    window_->DestroyWindow();
  }
  impl_->Cleanup();
}

void WindowManager::Tick() {
  window_->Tick();
}
   
std::optional<WindowRef> WindowManager::CreateWindow(int width, int height, 
                                                     const std::string& title) {
  window_ = internal::Window::Create();
  window_->CreateWindow(width, height, title);
  
  return WindowRef(0, window_.get());
}

} // namespace