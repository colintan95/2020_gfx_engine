#ifndef GAL_PLATFORM_H_
#define GAL_PLATFORM_H_

#include <exception>
#include <memory>
#include <optional>
#include "gal/command.h"

namespace gal {

class GALObject;

namespace internal {

// TODO(colintan): Rename to something that fits the meaning more
// Implement in platform-specific source files
class PlatformDetails;

class GALPlatformImpl {
public:
  virtual ~GALPlatformImpl() {}

  virtual PlatformDetails* GetPlatformDetails() = 0;

public:
  static std::unique_ptr<GALPlatformImpl> Create();
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
  GALPlatform();
  ~GALPlatform();

  template<typename T, typename... Args>
  std::optional<T> Create(Args... args) {
    return T::Create(this, args...);
  }

  void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf);

  internal::PlatformDetails* GetPlatformDetails() {
    return impl_->GetPlatformDetails();
  }

private:
  std::unique_ptr<internal::GALPlatformImpl> impl_;
};



} // namespace

#endif // GAL_PLATFORM_H_