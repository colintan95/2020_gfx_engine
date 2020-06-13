#ifndef WINDOW_WINDOW_INTERNAL_H_
#define WINDOW_WINDOW_INTERNAL_H_

#include <memory>
#include <string>

namespace window {

class WindowInternal {
public:
  virtual bool CreateWindow(int width, int height, const std::string& title) = 0;
  virtual void DestroyWindow() = 0;

  virtual ~WindowInternal() {}

  virtual void SwapBuffers() = 0;

public:
  static std::unique_ptr<WindowInternal> Create();
};

} // namespace

#endif // WINDOW_WINDOW_INTERNAL_H_