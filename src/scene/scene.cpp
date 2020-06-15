#include "scene/scene.h"

namespace scene {

bool Scene::Initialize(render::Renderer* renderer) {
  renderer_ = renderer;
  return true;
}

void Scene::Cleanup() {

}

void Scene::Tick() {
  
}

} // namespace