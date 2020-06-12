#include "platform/platform.h"

namespace platform {

Platform::Platform() {
  impl_ = CreatePlatformImpl();
  input_manager_ = std::make_unique<platform::InputManager>();
  window_ = std::make_unique<platform::Window>();
}

Platform::~Platform() {
  window_.reset();
  input_manager_.reset();
  impl_.reset();
}

bool Platform::Initialize() {
  if (!impl_->Initialize()) {
    return false;
  }
  if (!window_->Initialize()) {
    return false;
  }
  return true;
}
  
void Platform::Cleanup() {
  window_->Cleanup();
  input_manager_->Cleanup();
  impl_->Cleanup();
}

} // namespace