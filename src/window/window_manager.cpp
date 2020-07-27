#include "window/window_manager.h"

#include <memory>
#include "window/window_impl.h"

namespace window {

WindowManager::WindowManager() {
  impl_ = WindowManagerImpl::Create();
}

WindowManager::~WindowManager() {
  if (window_ != nullptr) {
    window_->DestroyWindow();
  }
  impl_.reset();
}

void WindowManager::Tick() {
  window_->Tick();
}

bool WindowManager::ShouldClose() {
  return window_->ShouldClose();
}
   
Window* WindowManager::CreateWindow(int width, int height, const std::string& title) {
  window_ = std::make_unique<Window>();
  window_->CreateWindow(width, height, title);
  return window_.get();
}

void WindowManager::DestroyWindow(Window* window) {
  window_->DestroyWindow();
  window_.reset();
}

} // namespace