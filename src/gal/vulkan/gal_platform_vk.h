#ifndef GAL_VULKAN_GAL_PLATFORM_VK_H_
#define GAL_VULKAN_GAL_PLATFORM_VK_H_

#include <vulkan/vulkan.h>

#include <memory>
#include "gal/gal_platform.h"

namespace gal {
namespace internal {

class PlatformDetails {

};

class GALPlatformImplVk : public GALPlatformImpl {
public:
  GALPlatformImplVk(window::Window* window);
  ~GALPlatformImplVk();

  PlatformDetails* GetPlatformDetails() final {
    return details_.get();
  }

private:
  std::unique_ptr<PlatformDetails> details_;

  VkInstance vk_instance_;
  VkDebugUtilsMessengerEXT vk_debug_messenger_;
  VkSurfaceKHR vk_surface_;
};

} // namespace
} // namespace

#endif // GAL_VULKAN_GAL_PLATFORM_VK_H_