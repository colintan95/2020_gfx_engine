#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <cstdint>
#include <optional>
#include "event/event.h"

namespace window {

namespace internal {
class Window;
} // namespace

using WindowId = uint32_t;

class WindowRef {
  friend class WindowManager;

public:
  WindowRef(WindowId window_id, internal::Window* impl);
  WindowRef() {}

  void SwapBuffers();

  // TODO(colintan): Consider a more flexible event listener approach instead
  std::optional<event::Event> ConsumeEvent();

  bool ShouldClose();

private:
  WindowId window_id_;
  internal::Window* impl_;
};

} // namespace

#endif // WINDOW_WINDOW_H_