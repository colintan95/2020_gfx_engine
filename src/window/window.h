#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <memory>
#include <string>

namespace input {
class InputManager;
} // namespace

namespace window {

class WindowImpl;

class Window {
public:
  Window(input::InputManager* input_manager);
  ~Window();

  bool Initialize(int width, int height, const std::string& title);
  void Destroy();

  void Tick();

  bool ShouldClose();

private:
  std::unique_ptr<WindowImpl> impl_;
  input::InputManager* input_manager_;
};

} // namespace

#endif // WINDOW_WINDOW_H_