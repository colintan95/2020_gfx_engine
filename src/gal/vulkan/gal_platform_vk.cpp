#include "gal/vulkan/gal_platform_vk.h"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <vector>

namespace gal {
namespace internal {

namespace {

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void* pUserData) {
  std::cerr << "Vulkan: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

} // namespace

GALPlatformImplVk::GALPlatformImplVk() {
  try {
    details_ = std::make_unique<PlatformDetails>();
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }

  // TODO(colintan): Enable validation layers
  VkApplicationInfo app_info = {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.apiVersion = VK_MAKE_VERSION(1, 0, 0);

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info= {};
  debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debug_create_info.messageSeverity = 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debug_create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_create_info.pfnUserCallback = debugCallback;

  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  std::vector<const char*> layers;
  layers.push_back("VK_LAYER_KHRONOS_validation");

  VkInstanceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pNext = &debug_create_info;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledLayerCount = layers.size();
  create_info.ppEnabledLayerNames = layers.data();
  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();
  
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