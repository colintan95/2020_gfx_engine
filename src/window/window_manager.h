#ifndef WINDOW_WINDOW_MANAGER_H_
#define WINDOW_WINDOW_MANAGER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <unordered_map>
#include "window/window.h"

namespace window {

class WindowInternal;

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

  std::optional<WindowRef> CreateWindow(int width, int height, const std::string& title);

private:
  std::unique_ptr<WindowManagerImpl> impl_;
  // std::unordered_map<WindowId, std::unique_ptr<WindowInternal>> windows_;

  // TODO(colintan): Support one window for now
  std::unique_ptr<WindowInternal> window_;
}; 

} // namespace

#endif // WINDOW_WINDOW_MANAGER_H_