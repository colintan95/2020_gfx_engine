#ifndef RENDER_RENDERER_H_
#define RENDER_RENDERER_H_

#include <memory>
#include "gal/platform.h"
#include "window/window.h"

namespace resource {
class ResourceManagerGAL;
} // namespace

namespace render {

class Renderer {
public:
  Renderer();
  ~Renderer();

  bool Initialize(window::Window* window);
  void Cleanup();

  void Tick();

private:  
  window::Window* window_ = nullptr;
  std::unique_ptr<resource::ResourceManagerGAL> resource_manager_;
  std::unique_ptr<gal::GALPlatform> gal_platform_;
  gal::GALCommandBuffer command_buffer_;
};

} // namespace

#endif // RENDER_RENDERER_H_