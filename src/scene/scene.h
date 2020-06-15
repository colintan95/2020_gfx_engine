#ifndef SCENE_SCENE_H_
#define SCENE_SCENE_H_

#include <vector>
#include "scene/entity.h"

namespace render {
class Renderer;
} // namespace

namespace scene {

class Scene {
public:
  bool Initialize(render::Renderer* renderer);
  void Cleanup();

  void Tick();

private:
  render::Renderer* renderer_;

  // TODO(colintan): Turn this into a scene graph
  std::vector<Entity> entities_;
};

} // namespace

#endif // SCENE_SCENE_H_