#ifndef GAL_GAL_PLATFORM_H_
#define GAL_GAL_PLATFORM_H_

#include <exception>
#include <memory>
#include <optional>
#include "gal/gal_command_buffer.h"

namespace window {
class Window;
} // namespace

namespace gal {

class GALObject;

namespace internal {

// TODO(colintan): Rename to something that fits the meaning more
// Implement in platform-specific source files
class PlatformDetails;

class GALPlatformImpl {
public:
  virtual ~GALPlatformImpl() {}

  virtual void Tick() = 0;

  virtual bool ExecuteComandBuffer(const GALCommandBuffer& command_buffer) = 0;

  virtual PlatformDetails* GetPlatformDetails() = 0;

public:
  static std::unique_ptr<GALPlatformImpl> Create(window::Window* window);
};

} // namespace

class GALPlatform {
friend class GALObject;

public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize GALPlatform.";
    }
  };

public:
  GALPlatform(window::Window* window);
  ~GALPlatform();

  template<typename T, typename... Args>
  std::optional<T> Create(Args... args) {
    return T::Create(this, args...);
  }

  void Tick() { impl_->Tick(); }

  bool ExecuteCommandBuffer(const GALCommandBuffer& command_buffer) {
    return impl_->ExecuteComandBuffer(command_buffer);
  }

  internal::PlatformDetails* GetPlatformDetails() {
    return impl_->GetPlatformDetails();
  }

private:
  std::unique_ptr<internal::GALPlatformImpl> impl_;
};

} // namespace

#endif // GAL_GAL_PLATFORM_H_