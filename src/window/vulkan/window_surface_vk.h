#ifndef WINDOW_VULKAN_WINDOW_SURFACE_H_
#define WINDOW_VULKAN_WINDOW_SURFACE_H_

// TODO(colintan): Don't expose these here
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <exception>

// class VkSurfaceKHR;
// class VkInstance;

namespace window {

class WindowSurface {
public:
  struct CreateInfo {
    VkInstance vk_instance;
  };

  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize WindowSurface.";
    }
  };

public:
  WindowSurface(CreateInfo create_info, GLFWwindow* glfw_window);

  VkSurfaceKHR GetVkSurface() { return vk_surface_; }

private:
  VkSurfaceKHR vk_surface_;
  VkInstance vk_instance_;
};



} // namespace window

#endif // WINDOW_WINDOW_SURFACE_H_