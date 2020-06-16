#ifndef GAL_OPENGL_GAL_PLATFORM_GL_H_
#define GAL_OPENGL_GAL_PLATFORM_GL_H_

#include <memory>
#include "gal/gal_platform.h"

namespace gal {
namespace internal {

class PlatformDetails {

};

class GALPlatformImplGL : public GALPlatformImpl {
public:
  GALPlatformImplGL();

  PlatformDetails* GetPlatformDetails() final {
    return details_.get();
  }

private:
  std::unique_ptr<PlatformDetails> details_;
};

} // namespace
} // namespace

#endif // GAL_OPENGL_GAL_PLATFORM_GL_H_