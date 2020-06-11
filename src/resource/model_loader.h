#ifndef RESOURCE_MODEL_LOADER_H_
#define RESOURCE_MODEL_LOADER_H_

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace resource {

struct Model {
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texcoords;
};

class ModelLoader {
public:
  std::shared_ptr<Model> LoadModel(const std::string& path);
};

} // namespace resource

#endif // RESOURCE_MODEL_LOADER_H_