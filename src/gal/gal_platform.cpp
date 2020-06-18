#include "gal/gal_platform.h"

namespace gal {

GALPlatform::GALPlatform(window::Window* window) {
  impl_ = internal::GALPlatformImpl::Create(window);
}

GALPlatform::~GALPlatform() {
  impl_.reset();
}

// void GALPlatform::ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
//   cmd_buf.Execute();
// }

} // namespace