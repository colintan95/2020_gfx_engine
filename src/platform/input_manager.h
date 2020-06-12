#ifndef INPUT_INPUT_MANAGER_H_
#define INPUT_INPUT_MANAGER_H_

#include <memory>
#include <variant>
#include <vector>

namespace platform {
namespace internal {
class ImplCreator;
} // namespace
} // namespace

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

using Event = 
    std::variant<Keyboard, MouseButton, MouseWheel>;

} // namespace

class InputManager {
public:
  InputManager(platform::internal::ImplCreator* impl_creator);
  ~InputManager();

  bool Initialize();
  void Cleanup();

  void Tick();

private:
  // For the input source class
  void AddEvent(event::Event event);

private:
  std::unique_ptr<internal::InputSource> input_source_;
  std::vector<event::Event> pending_events_;
};

} // namespace

#endif // INPUT_INPUT_MANAGER_H_