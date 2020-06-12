#ifndef PLATFORM_INPUT_SOURCE_H_
#define PLATFORM_INPUT_SOURCE_H_

#include <memory>

namespace input {
class InputManager;

namespace internal {

class InputSource {
  friend class InputManager;

public:
  virtual ~InputSource() {}

  virtual bool Initialize() = 0;
  virtual void Cleanup() = 0;

  virtual void Tick() = 0;

private:
  void SetInputManager(InputManager* input_manager) {
    input_manager_ = input_manager;
  }

protected:
  InputManager* input_manager_;
};

} // namespace
} // namespace

#endif // PLATFORM_INPUT_SOURCE_H_