#ifndef WINDOW_WINDOW_INTERNAL_H_
#define WINDOW_WINDOW_INTERNAL_H_

#include <memory>
#include <string>
#include <optional>
#include "window/event.h"

namespace window {
namespace internal {

class Window {
public:
  virtual bool CreateWindow(int width, int height, const std::string& title) = 0;
  virtual void DestroyWindow() = 0;

  virtual ~Window() {}

  virtual void Tick() = 0;

  virtual void SwapBuffers() = 0;

  virtual bool ShouldClose() = 0;

  virtual std::optional<Event> ConsumeEvent() = 0;

public:
  static std::unique_ptr<Window> Create();
};

} // namespace
} // namespace

#endif // WINDOW_WINDOW_INTERNAL_H_