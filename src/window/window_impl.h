#ifndef WINDOW_WINDOW_IMPL_H_
#define WINDOW_WINDOW_IMPL_H_

#include <memory>
#include <string>
#include <optional>
#include "event/event.h"

namespace window {
namespace internal {

class WindowImpl {
public:
  virtual bool CreateWindow(int width, int height, const std::string& title) = 0;
  virtual void DestroyWindow() = 0;

  virtual ~WindowImpl() {}

  virtual void Tick() = 0;

  virtual void SwapBuffers() = 0;

  virtual bool ShouldClose() = 0;

  virtual std::optional<event::Event> ConsumeEvent() = 0;

public:
  static std::unique_ptr<WindowImpl> Create();
};

} // namespace
} // namespace

#endif // WINDOW_WINDOW_IMPL_H_