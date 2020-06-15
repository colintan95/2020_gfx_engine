#ifndef SCENE_ENTITY_H_
#define SCENE_ENTITY_H_

#include <glm/glm.hpp>
#include "render/model.h"

namespace scene {

class Entity {
private:
  glm::mat4 local_transform_;

  render::ModelId model_id_;
};

} // namespace

#endif // SCENE_ENTITY_H_