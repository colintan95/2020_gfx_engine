#include "window/vulkan/window_surface_vk.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace window {

WindowSurface::WindowSurface(CreateInfo create_info, GLFWwindow* glfw_window) {
  if (glfwCreateWindowSurface(create_info.vk_instance, glfw_window, nullptr,
                              &vk_surface_) != VK_SUCCESS) {
    throw InitException();
  }
  vk_instance_ = create_info.vk_instance;
}

WindowSurface::~WindowSurface() {
  vkDestroySurfaceKHR(vk_instance_, vk_surface_, nullptr);
}

} // namespace