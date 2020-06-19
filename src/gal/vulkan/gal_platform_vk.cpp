#include "gal/vulkan/gal_platform_vk.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <optional>
#include <unordered_set>
#include <utility>
#include <vector>
#include "window/window.h"

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

GALPlatformImplVk::GALPlatformImplVk(window::Window* window) {
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

  std::vector<const char*> validation_layers;
  validation_layers.push_back("VK_LAYER_KHRONOS_validation");

  VkInstanceCreateInfo instance_create_info = {};
  instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instance_create_info.pNext = &debug_create_info;
  instance_create_info.pApplicationInfo = &app_info;
  instance_create_info.enabledLayerCount = validation_layers.size();
  instance_create_info.ppEnabledLayerNames = validation_layers.data();
  instance_create_info.enabledExtensionCount = extensions.size();
  instance_create_info.ppEnabledExtensionNames = extensions.data();
  
  if (vkCreateInstance(&instance_create_info, nullptr, &vk_instance_) != VK_SUCCESS) {
    std::cerr << "Could not create VkInstance." << std::endl;
    throw GALPlatform::InitException();
  }

  auto create_debug_utils_messenger_func = 
      (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            vk_instance_, "vkCreateDebugUtilsMessengerEXT");
  if (create_debug_utils_messenger_func == nullptr) {
    std::cerr << "Could not find create debug utils messenger func." << std::endl;
    throw GALPlatform::InitException();
  }
  if (create_debug_utils_messenger_func(
        vk_instance_, &debug_create_info, nullptr, &vk_debug_messenger_) != VK_SUCCESS) {
    std::cerr << "Could not create debug utils messenger." << std::endl;
    throw GALPlatform::InitException();
  }

  window::WindowSurface::CreateInfo window_surface_create_info;
  window_surface_create_info.vk_instance = vk_instance_;
  if (!window->CreateWindowSurface(window_surface_create_info)) {
    std::cerr << "Could not create WindowSurface" << std::endl;
    throw GALPlatform::InitException();
  }

  vk_surface_ = window->GetWindowSurface()->GetVkSurface();

  std::optional<PhysicalDeviceInfo> physical_device_info = ChoosePhysicalDevice();

  if (!physical_device_info.has_value()) {
    std::cerr << "Could not find a suitable physical device." << std::endl;
    throw GALPlatform::InitException();
  }

  uint32_t graphics_queue_family_index = physical_device_info.value().graphics_queue_family_index;
  uint32_t present_queue_family_index = physical_device_info.value().present_queue_family_index;

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

  std::unordered_set<uint32_t> queue_family_indices = { graphics_queue_family_index, 
                                                        present_queue_family_index };

  for (uint32_t queue_family_index : queue_family_indices) {
    float queue_properties[] = { 1.f };

    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = queue_properties;

    queue_create_infos.push_back(std::move(queue_create_info));
  }
  
  VkPhysicalDeviceFeatures device_enabled_features{};

  std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

  VkDeviceCreateInfo device_create_info{};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.queueCreateInfoCount = queue_create_infos.size();
  device_create_info.pQueueCreateInfos = queue_create_infos.data();
  device_create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
  device_create_info.ppEnabledLayerNames = validation_layers.data();
  device_create_info.enabledExtensionCount = device_extensions.size();
  device_create_info.ppEnabledExtensionNames = device_extensions.data();
  device_create_info.pEnabledFeatures = &device_enabled_features;
  
  if (vkCreateDevice(vk_physical_device_, &device_create_info, nullptr, 
                     &vk_device_) != VK_SUCCESS) {
    std::cerr << "Could not create VkDevice." << std::endl;
    throw GALPlatform::InitException();
  }

  vkGetDeviceQueue(vk_device_, graphics_queue_family_index, 0, &vk_graphics_queue_);
  vkGetDeviceQueue(vk_device_, present_queue_family_index, 0, &vk_present_queue_);
}

GALPlatformImplVk::~GALPlatformImplVk() {
  vkDestroyDevice(vk_device_, nullptr);

  vkDestroySurfaceKHR(vk_instance_, vk_surface_, nullptr);

  auto destroy_debug_utils_messenger_func = 
      (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vk_instance_, 
          "vkDestroyDebugUtilsMessengerEXT");
  if (destroy_debug_utils_messenger_func != nullptr) {
    destroy_debug_utils_messenger_func(vk_instance_, vk_debug_messenger_, nullptr);
  }

  vkDestroyInstance(vk_instance_, nullptr);
}

std::optional<GALPlatformImplVk::PhysicalDeviceInfo> GALPlatformImplVk::ChoosePhysicalDevice() {
  uint32_t physical_devices_count = 0;
  vkEnumeratePhysicalDevices(vk_instance_, &physical_devices_count, nullptr);

  std::vector<VkPhysicalDevice> physical_devices(physical_devices_count);
  vkEnumeratePhysicalDevices(vk_instance_, &physical_devices_count, physical_devices.data());

  bool found_physical_device = false;
  std::optional<uint32_t> graphics_queue_family_index;
  std::optional<uint32_t> present_queue_family_index;

  for (const VkPhysicalDevice& device : physical_devices) {
    VkPhysicalDeviceProperties device_props;
    vkGetPhysicalDeviceProperties(device, &device_props);
    
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(device, &device_features);

    if (device_props.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      continue;
    }

    uint32_t queue_families_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_families_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_families_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_families_count, queue_families.data());

    bool found_queues = false;
    int index = 0;
    for (const VkQueueFamilyProperties& queue_family : queue_families) {
      if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        graphics_queue_family_index = index;
      }

      VkBool32 supports_present = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, index, vk_surface_, 
                                           &supports_present);
      if (supports_present) {
        present_queue_family_index = index;
      }

      if (graphics_queue_family_index.has_value() && present_queue_family_index.has_value()) {
        found_queues = true;
        break;
      }
        
      ++index;
    }
    if (!found_queues) {
      continue;
    }

    uint32_t extensions_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensions_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extensions_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensions_count, extensions.data());

    // TODO(colintan): Enumerate through a list of requested extensions and see if the physical
    // device supports each.
    if (std::find_if(extensions.begin(), extensions.end(), 
            [](const VkExtensionProperties& props) { 
              return strncmp(props.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                             sizeof(VK_KHR_SWAPCHAIN_EXTENSION_NAME)) == 0;
            }) == extensions.end()) {
      continue;
    }

    found_physical_device = true;
    vk_physical_device_ = device;
  }
  
  if (!found_physical_device) {
    return std::nullopt;
  }

  assert(present_queue_family_index.has_value());
  assert(present_queue_family_index.has_value());

  PhysicalDeviceInfo result;
  result.graphics_queue_family_index = graphics_queue_family_index.value();
  result.present_queue_family_index = present_queue_family_index.value();

  return result;
}

std::unique_ptr<GALPlatformImpl> GALPlatformImpl::Create(window::Window* window) {
  try {
    return std::make_unique<GALPlatformImplVk>(window);
  } catch (std::bad_alloc& ba) {
    throw GALPlatform::InitException();
  }
}

} // namespace
} // namespace