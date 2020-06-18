#include "gal/vulkan/gal_platform_vk.h"

namespace gal {
namespace internal {

GALPlatformImplVk::GALPlatformImplVk() {
  try {
    details_ = std::make_unique<PlatformDetails>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }


}

std::unique_ptr<GALPlatformImpl> GALPlatformImpl::Create() {
  try {
    return std::make_unique<GALPlatformImplVk>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }
}

} // namespace
} // namespace