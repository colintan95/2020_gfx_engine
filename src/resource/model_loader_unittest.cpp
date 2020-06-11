#include "model_loader.h"

#include <glm/glm.hpp>

#include <vector>
#include "gtest/gtest.h"

namespace resource {

TEST(ModelLoaderTest, LoadSquare) {
  ModelLoader loader;
  std::shared_ptr<Model> model = loader.LoadModel("test.obj");

  EXPECT_NE(model, nullptr);
  EXPECT_EQ(model->positions.size(), 6);
  EXPECT_EQ(model->normals.size(), 6);
  EXPECT_EQ(model->texcoords.size(), 6);

  const std::vector<glm::vec3> positions = {
    {-1.f, -1.f, 1.f}, {-1.f, 1.f, 1.f}, {1.f, -1.f, 1.f},
    {1.f, -1.f, 1.f}, {-1.f, 1.f, 1.f}, {1.f, 1.f, 1.f}
  };
  EXPECT_EQ(model->positions, positions);

  const std::vector<glm::vec3> normals = {
    {-1.f, 0.f, 0.f}, {-1.f, 0.f, 0.f}, {-1.f, 0.f, 0.f},
    {-1.f, 0.f, 0.f}, {-1.f, 0.f, 0.f}, {-1.f, 0.f, 0.f}
  };
  EXPECT_EQ(model->normals, normals);

  const std::vector<glm::vec2> texcoords = {
    {0.f, 0.f}, {0.f, 1.f}, {1.f, 0.f}, {1.f, 0.f}, {0.f, 1.f}, {1.f, 1.f}
  };
  EXPECT_EQ(model->texcoords, texcoords);
}

} // namespace resource