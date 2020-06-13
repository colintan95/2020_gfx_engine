#include "gal/opengl/platform_gl.h"

#include <GL/glew.h>

#include <memory>

namespace gal {
namespace internal {

GALPlatformImplGL::GALPlatformImplGL() {
  details_ = std::make_unique<PlatformDetails>();
}

bool GALPlatformImplGL::Initialize() {
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  return true;
}

void GALPlatformImplGL::Cleanup() {

}

std::unique_ptr<GALPlatformImpl> GALPlatformImpl::Create() {
  return std::make_unique<GALPlatformImplGL>();
}

} // namespace
} // namespace