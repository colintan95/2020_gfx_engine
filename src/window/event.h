#ifndef WINDOW_EVENTS_H_
#define WINDOW_EVENTS_H_

#include <variant>

namespace window {

enum class KeyInput {
  None = 0,
  KeyA,
  KeyW,
  Key1
};

enum class KeyAction {
  None = 0,
  Down,
  Up
};

enum class MouseInput {
  None = 0,
  ButtonLeft,
  ButtonRight
};

enum class MouseAction {
  None = 0,
  Click,
  DoubleClick,
  Down, 
  Up
};

struct KeyboardEvent {
  KeyInput key;
  KeyAction action;
  KeyInput modifier_key;
};

struct MouseButtonEvent {
  MouseInput button;
  MouseAction action;
  double mouse_x;
  double mouse_y;
};

struct MouseWheelEvent {
  double wheel_delta;
  double mouse_x;
  double mouse_y;
  KeyInput modifier_key;
};

using EventData = 
    std::variant<KeyboardEvent, 
                 MouseButtonEvent, 
                 MouseWheelEvent>;

class Event {
public:
  template<typename T>
  bool IsType() const { return std::holds_alternative<T>(data_); }

  template<typename T>
  const T& AsType() const { return std::get<T>(data_); }

  static Event Create(EventData data) { 
    Event evt;
    evt.data_ = data;
    return evt;
  }

private:
  EventData data_;
};

} // namespace

#endif // WINDOW_EVENTS_H_