#ifndef GAL_GAL_SHADER_H_
#define GAL_GAL_SHADER_H_

#include "gal/object_impl/gal_shader_impl.h"

#if defined(GFXAPI_GL)
#include "gal/opengl/object_impl/gal_shader_impl_gl.h"
#endif

#if defined(GFXAPI_VK)
#include "gal/vulkan/object_impl/gal_shader_impl_vk.h"
#endif

#endif //GAL_GAL_SHADER_H_