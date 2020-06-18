#include "gal/vulkan/gal_platform_vk.h"

namespace gal {
namespace internal {

GALPlatformImplVk::GALPlatformImplVk() {
  try {
    details_ = std::make_unique<PlatformDetails>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }

  // TODO(colintan): Enable validation layers
  VkApplicationInfo app_info;
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = nullptr; // TODO(colintan): Set it to the proper name
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = nullptr;
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

  VkInstanceCreateInfo create_info;
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pNext = nullptr;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledLayerCount = 0;
  create_info.ppEnabledLayerNames = nullptr;
  create_info.enabledExtensionCount = 0;
  create_info.ppEnabledExtensionNames = nullptr;

  // TODO(colintan): Load extensions

  if (vkCreateInstance(&create_info, nullptr, &vk_instance_) != VK_SUCCESS) {
    throw GALPlatform::InitException();
  }
}

GALPlatformImplVk::~GALPlatformImplVk() {
  vkDestroyInstance(vk_instance_, nullptr);
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