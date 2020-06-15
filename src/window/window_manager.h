#ifndef WINDOW_WINDOW_MANAGER_H_
#define WINDOW_WINDOW_MANAGER_H_

#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <optional>
#include "window/window.h"

namespace window {

class WindowManagerImpl {
public:
  virtual ~WindowManagerImpl() {}

public:
  static std::unique_ptr<WindowManagerImpl> Create();
};

class WindowManager {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize WindowManager.";
    }
  };

public:
  WindowManager();
  ~WindowManager();
  
  void Tick();

  bool ShouldClose();

  // TODO(colintan): Support multiple windows
  Window* CreateWindow(int width, int height, const std::string& title);
  void DestroyWindow(Window* window);

private:
  std::unique_ptr<WindowManagerImpl> impl_;

  // TODO(colintan): Support one window for now
  std::unique_ptr<Window> window_;
}; 

} // namespace

#endif // WINDOW_WINDOW_MANAGER_H_