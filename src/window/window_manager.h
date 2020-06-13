#ifndef WINDOW_WINDOW_MANAGER_H_
#define WINDOW_WINDOW_MANAGER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include "window/window.h"
#include "window/window_internal.h"

namespace window {

class WindowManagerImpl {
public:
  virtual ~WindowManagerImpl() {}

  virtual bool Initialize() = 0;
  virtual void Cleanup() = 0;

public:
  static std::unique_ptr<WindowManagerImpl> Create();
};

class WindowManager {
public:
  WindowManager();
  ~WindowManager();

  bool Initialize();
  void Cleanup();

  void Tick();

  bool ShouldClose();

  std::optional<WindowRef> CreateWindow(int width, int height, const std::string& title);

private:
  std::unique_ptr<WindowManagerImpl> impl_;

  // TODO(colintan): Support one window for now
  std::unique_ptr<internal::Window> window_;
}; 

} // namespace

#endif // WINDOW_WINDOW_MANAGER_H_