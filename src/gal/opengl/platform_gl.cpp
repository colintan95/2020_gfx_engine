#include "gal/opengl/platform_gl.h"

#include <GL/glew.h>

#include <memory>
#include <new>
#include "gal/platform.h"

namespace gal {
namespace internal {

GALPlatformImplGL::GALPlatformImplGL() {
  try {
    details_ = std::make_unique<PlatformDetails>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

std::unique_ptr<GALPlatformImpl> GALPlatformImpl::Create() {
  try {
    return std::make_unique<GALPlatformImplGL>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }
}

} // namespace
} // namespace