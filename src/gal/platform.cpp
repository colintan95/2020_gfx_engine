#include "gal/platform.h"

namespace gal {

GALPlatform::GALPlatform() {
  impl_ = internal::GALPlatformImpl::Create();
}

GALPlatform::~GALPlatform() {
  impl_.reset();
}

bool GALPlatform::Initialize() {
  return impl_->Initialize();
}

void GALPlatform::Cleanup() {
  impl_->Cleanup();
}

void GALPlatform::ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  cmd_buf.Execute();
}

} // namespace