#ifndef SCENE_ENTITY_H_
#define SCENE_ENTITY_H_

#include <glm/glm.hpp>

namespace scene {

class Entity {
private:
  glm::mat4 local_transform_;
};

} // namespace

#endif // SCENE_ENTITY_H_