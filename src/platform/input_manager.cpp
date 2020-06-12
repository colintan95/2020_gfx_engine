#include "platform/input_manager.h"

#include "platform/input_source.h"

namespace platform {

InputManager::InputManager() {
  input_source_ = internal::CreateInputSource();
}

InputManager::~InputManager() {
  input_source_.reset();
}

bool InputManager::Initialize() {
  return false;
}

void InputManager::Cleanup() {
  
}

} // namespace