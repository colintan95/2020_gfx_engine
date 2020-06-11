#include "model_loader.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

namespace resource {

std::shared_ptr<Model> ModelLoader::LoadModel(const std::string& path) {
  tinyobj::ObjReader obj_reader;

  if (!obj_reader.ParseFromFile(path)) {
    std::cerr << "Failed to load model from path: " << path << std::endl;
    return nullptr;
  }

  const tinyobj::attrib_t& attrib = obj_reader.GetAttrib(); 
  const std::vector<tinyobj::shape_t>& shapes = obj_reader.GetShapes();
  const std::vector<tinyobj::material_t>& materials = obj_reader.GetMaterials();

  auto model = std::make_shared<Model>();

  for (const tinyobj::shape_t& shape : shapes) {
    const tinyobj::mesh_t& mesh = shape.mesh;

    // TODO(colintan): Support num_face_vertices > 3
    for (int num_face : mesh.num_face_vertices) {
      assert(num_face == 3);
    }

    model->faces = mesh.num_face_vertices.size();

    for (tinyobj::index_t vert_indices : mesh.indices) {
      int pos_idx = vert_indices.vertex_index * 3;
      int norm_idx = vert_indices.normal_index * 3;
      int tex_idx = vert_indices.texcoord_index * 2;

      const std::vector<tinyobj::real_t>& positions = attrib.vertices;
      const std::vector<tinyobj::real_t>& normals = attrib.normals;
      const std::vector<tinyobj::real_t>& texcoords = attrib.texcoords;

      model->positions.push_back(glm::vec3{positions[pos_idx + 0], 
                                           positions[pos_idx + 1],
                                           positions[pos_idx + 2]});
      model->normals.push_back(glm::vec3{normals[norm_idx + 0],
                                         normals[norm_idx + 1],
                                         normals[norm_idx + 2]});
      model->texcoords.push_back(glm::vec2{texcoords[tex_idx + 0],
                                           texcoords[tex_idx + 1]});                                   
    }
  }

  return model;
}

} // namespace