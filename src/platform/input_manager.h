#ifndef INPUT_INPUT_MANAGER_H_
#define INPUT_INPUT_MANAGER_H_

#include <memory>

namespace input {

namespace internal {
class InputSource;
} // namespace

enum class KeyInput {
  None,
  KeyW,
  Key1
};

enum class KeyAction {
  Down,
  Up
};

enum class MouseInput {
  None,
  ButtonLeft,
  ButtonRight
};

enum class MouseAction {
  Click,
  DoubleClick,
  Down, 
  Up
};


namespace event {

struct Keyboard {
  KeyInput key;
  KeyAction action;
  KeyInput modifier_key;
};

struct MouseButton {
  MouseInput button;
  MouseAction action;
  double mouse_x;
  double mouse_y;
};

struct MouseWheel {
  double wheel_delta;
  double mouse_x;
  double mouse_y;
  KeyInput modifier_key;
};

} // namespace

class InputManager {
public:
  InputManager();
  ~InputManager();

  bool Initialize();
  void Cleanup();

  void Tick();

private:
  std::unique_ptr<internal::InputSource> input_source_;
};

} // namespace

#endif // INPUT_INPUT_MANAGER_H_