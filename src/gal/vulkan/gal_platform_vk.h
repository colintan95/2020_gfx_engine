#ifndef GAL_VULKAN_GAL_PLATFORM_VK_H_
#define GAL_VULKAN_GAL_PLATFORM_VK_H_

#include <vulkan/vulkan.h>

#include <memory>
#include <optional>
#include <vector>
#include "gal/gal_platform.h"

namespace gal {
namespace internal {

struct PlatformDetails {
  VkDevice vk_device;
};

class GALPlatformImplVk : public GALPlatformImpl {
public:
  GALPlatformImplVk(window::Window* window);
  ~GALPlatformImplVk();

  PlatformDetails* GetPlatformDetails() final {
    return details_.get();
  }

private:
  struct PhysicalDeviceInfo {
    uint32_t graphics_queue_family_index;
    uint32_t present_queue_family_index;
  };

  std::optional<PhysicalDeviceInfo> ChoosePhysicalDevice();

  VkSurfaceFormatKHR ChooseSurfaceFormat();
  VkPresentModeKHR ChoosePresentMode();
  VkExtent2D ChooseSwapExtent();

private:
  std::unique_ptr<PlatformDetails> details_;

  window::Window* window_;

  VkInstance vk_instance_;
  VkDebugUtilsMessengerEXT vk_debug_messenger_;
  VkSurfaceKHR vk_surface_;
  VkPhysicalDevice vk_physical_device_;
  VkDevice vk_device_;
  VkQueue vk_graphics_queue_;
  VkQueue vk_present_queue_;

  VkSwapchainKHR vk_swapchain_;
  VkFormat vk_swapchain_image_format_;
  VkExtent2D vk_swapchain_extent_;

  std::vector<VkImage> vk_swapchain_images_;
  std::vector<VkImageView> vk_swapchain_image_views_;
};

} // namespace
} // namespace

#endif // GAL_VULKAN_GAL_PLATFORM_VK_H_