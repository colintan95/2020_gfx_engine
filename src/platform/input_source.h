#ifndef PLATFORM_INPUT_SOURCE_H_
#define PLATFORM_INPUT_SOURCE_H_

#include <memory>

namespace platform {
namespace internal {

class InputSource {
public:
  virtual ~InputSource() {}

};

// Platform-specific
std::unique_ptr<InputSource> CreateInputSource();

} // namespace
} // namespace

#endif // PLATFORM_INPUT_SOURCE_H_