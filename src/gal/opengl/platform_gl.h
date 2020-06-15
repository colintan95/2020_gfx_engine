#ifndef GAL_OPENGL_PLATFORM_GL_H_
#define GAL_OPENGL_PLATFORM_GL_H_

#include <memory>
#include "gal/platform.h"
#include "gal/opengl/id_converter.h"

namespace gal {
namespace internal {

class PlatformDetails {
public:
  void AddGALId(GALId gal_id, GLuint gl_id) { id_converter_.AddGALId(gal_id, gl_id); }
  void RemoveGALId(GALId gal_id) { id_converter_.RemoveGALId(gal_id); }

  std::optional<GLuint> ConvertGALId(GALId gal_id) { return id_converter_.ConvertGALId(gal_id); }

private:
  IdConverter id_converter_;
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

#endif // GAL_OPENGL_PLATFORM_GL_H_