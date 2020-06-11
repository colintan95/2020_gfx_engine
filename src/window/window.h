#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <memory>
#include <stdexcept>
#include <string>

namespace window {

// TODO(colintan): Append WindowInitException at beginning
class WindowInitException : public std::runtime_error {
public:
  WindowInitException(const std::string& msg) : std::runtime_error(msg) {}
}; 

class WindowImpl;

class Window {
public:
  Window(int width, int height, const std::string& title);
  ~Window();

  void Tick();

  bool ShouldClose();

private:
  std::unique_ptr<WindowImpl> impl_;
};

} // namespace

#endif // WINDOW_WINDOW_H_