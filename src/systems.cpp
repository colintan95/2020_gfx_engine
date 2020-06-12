#include "systems.h"

#include <memory>

bool Systems::InitSystems() {
  input_manager_ = std::make_unique<input::InputManager>();
  window_ = std::make_unique<window::Window>(input_manager_.get());

  return true;
}

void Systems::DestroySystems() {
  window_.reset();
  input_manager_.reset();
}