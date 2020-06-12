#include "platform/platform.h"

#include "platform/impl_creator.h"

namespace platform {

Platform::Platform() {
  impl_creator_ = internal::CreateImplCreator();

  impl_ = impl_creator_->CreatePlatformImpl();
  input_manager_ = std::make_unique<input::InputManager>(impl_creator_.get());
  window_ = std::make_unique<window::Window>(impl_creator_.get());
}

Platform::~Platform() {
  window_.reset();
  input_manager_.reset();
  impl_.reset();

  impl_creator_.reset();
}

bool Platform::Initialize() {
  if (!impl_->Initialize()) {
    return false;
  }
  if (!input_manager_->Initialize()) {
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

void Platform::Tick() {
  input_manager_->Tick();
  impl_->Tick();
}

} // namespace