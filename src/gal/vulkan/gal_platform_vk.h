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
  VkExtent2D vk_swapchain_extent;
  VkFormat vk_swapchain_image_format;
  std::vector<VkImageView> vk_swapchain_image_views;
  VkCommandPool vk_command_pool;
};

const int kMaxFramesInFlight = 2;

class GALPlatformImplVk : public GALPlatformImpl {
public:
  GALPlatformImplVk(window::Window* window);
  ~GALPlatformImplVk();

  void StartTick() final;
  void EndTick() final;

  bool ExecuteComandBuffer(const GALCommandBuffer& command_buffer) final;

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

  VkCommandPool vk_command_pool_;

  std::vector<VkImage> vk_swapchain_images_;
  std::vector<VkImageView> vk_swapchain_image_views_;

  std::vector<VkSemaphore> vk_image_available_semaphores_;
  std::vector<VkSemaphore> vk_render_finished_semaphores_;
  std::vector<VkFence> vk_in_flight_fences_;
  std::vector<VkFence> vk_images_in_flight_;

  uint32_t current_image_index_ = 0;
  uint32_t current_frame_ = 0;
};

} // namespace
} // namespace

#endif // GAL_VULKAN_GAL_PLATFORM_VK_H_