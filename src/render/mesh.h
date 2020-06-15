#ifndef RENDER_MESH_H_
#define RENDER_MESH_H_

#include <cstdint>
#include "resource/resource_gal.h"

namespace render {

using MeshId = uint32_t;

struct Mesh {
  resource::HandleGALBuffer pos_buf_;
  resource::HandleGALBuffer normal_buf_;
  resource::HandleGALBuffer texcoord_buf_;
};

} // namespace

#endif // RENDER_MESH_H_