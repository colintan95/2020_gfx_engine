#include "platform/input_manager.h"

#include "platform/impl_creator.h"
#include "platform/input_source.h"

namespace input {

InputManager::InputManager(platform::internal::ImplCreator* impl_creator) {
  input_source_ = impl_creator->CreateInputSource();
  input_source_->SetInputManager(this);
}

InputManager::~InputManager() {
  input_source_.reset();
}

bool InputManager::Initialize() {
  if (!input_source_->Initialize()) {
    return false;
  }
  return true;
}

void InputManager::Cleanup() {
  
}

void InputManager::Tick() {
  // TODO(colintan): Consider doing the tick somewhere else
  input_source_->Tick();
}

void InputManager::AddEvent(event::Event event) {
  pending_events_.push_back(event);
}

} // namespace