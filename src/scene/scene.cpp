#include "scene/scene.h"

namespace scene {

Scene::Scene(render::Renderer* renderer) {
  renderer_ = renderer;
}

Scene::~Scene() {
  renderer_ = nullptr;
}

void Scene::Tick() {
  
}

} // namespace