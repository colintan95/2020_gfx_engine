#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

#include <cstdint>
#include <optional>
#include <memory>
#include <string>
#include "event/event.h"

namespace window {

namespace internal {
class WindowImpl;
} // namespace

class Window {
friend class WindowManager;
friend class EventConsumer;

public:
  // TODO(colintan): Consider making this private (see abseil lib's WrapUnique) -
  // https://abseil.io/tips/134
  Window();

  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  void SwapBuffers();

  // TODO(colintan): Consider a more flexible event listener approach instead
  std::optional<event::Event> ConsumeEvent();

  bool ShouldClose();

private:
  bool CreateWindow(int width, int height, const std::string& title);
  void DestroyWindow();

  void Tick();

  void AddEventConsumer(EventConsumer* consumer);
  void RemoveEventConsumer(EventConsumer* consumer);

private:
  std::unique_ptr<internal::WindowImpl> impl_;
};

} // namespace

#endif // WINDOW_WINDOW_H_