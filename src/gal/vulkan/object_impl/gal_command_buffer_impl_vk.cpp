#include "gal/vulkan/object_impl/gal_command_buffer_impl_vk.h"

#include <iostream>
#include "gal/commands.h"
#include "gal/vulkan/gal_platform_vk.h"

namespace gal {
   
GALCommandBufferImplVk::Builder::ReturnType GALCommandBufferImplVk::Builder::Create() {
  ReturnType res;
  res.GetImpl().CreateFromBuilder(*this);
  res.SetValid(true);
  return res;
}

void GALCommandBufferImplVk::Destroy() {

}

void GALCommandBufferImplVk::AddCommand(const command::CommandUnion& command) {

}

void GALCommandBufferImplVk::CreateFromBuilder(GALCommandBufferImplVk::Builder& builder) {
  
}
   
} // namespace