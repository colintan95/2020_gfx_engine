#ifndef PLATFORM_WINDOW_H_
#define PLATFORM_WINDOW_H_

#include <memory>
#include <string>

namespace platform {
namespace internal {
class ImplCreator;
} // namespace
} // namespace

namespace window {

class WindowImpl {
public:
  virtual ~WindowImpl() {}

  virtual bool Initialize() = 0;
  virtual void Cleanup() = 0;

  virtual bool CreateWindow(int width, int height, const std::string& title) = 0;

  virtual void Tick() = 0;

  virtual bool ShouldClose() = 0;
};

class Window {
public:
  Window(platform::internal::ImplCreator* impl_creator);
  ~Window();

  bool Initialize() { return impl_->Initialize(); }
  void Cleanup() { impl_->Cleanup(); }

  bool CreateWindow(int width, int height, const std::string& title) {
    return impl_->CreateWindow(width, height, title);
  }

  void Tick() { impl_->Tick(); }

  bool ShouldClose() { return impl_->ShouldClose(); }

private:
  std::unique_ptr<WindowImpl> impl_;
};

} // namespace

#endif // PLATFORM_WINDOW_H_