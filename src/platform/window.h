#ifndef PLATFORM_WINDOW_H_
#define PLATFORM_WINDOW_H_

#include <memory>
#include <string>

namespace platform {

class WindowImpl;
class InputManager;

class Window {
public:
  Window(InputManager* input_manager);
  ~Window();

  bool Initialize(int width, int height, const std::string& title);
  void Destroy();

  void Tick();

  bool ShouldClose();

private:
  std::unique_ptr<WindowImpl> impl_;
  InputManager* input_manager_;
};

} // namespace

#endif // PLATFORM_WINDOW_H_