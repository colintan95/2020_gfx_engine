#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <cstdint>

namespace window {

class WindowInternal;

using WindowId = uint32_t;

class WindowRef {
  friend class WindowManager;

public:
  // SwapBuffer()

  // BindEventHandler()

  // ResizeWindow()

private:
  WindowRef(WindowId window_id, WindowInternal* impl);

private:
  WindowId window_id_;
  WindowInternal* impl_;
};

} // namespace

#endif // WINDOW_WINDOW_H_