#ifndef RENDER_RENDERER_H_
#define RENDER_RENDERER_H_

#include <memory>

namespace render {

class ResourceManager;

class Renderer {
public:
  Renderer();
  ~Renderer();

private:
  std::unique_ptr<ResourceManager> resource_manager_;
};

} // namespace

#endif // RENDER_RENDERER_H_