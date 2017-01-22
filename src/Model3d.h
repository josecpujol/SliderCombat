#pragma once

#include "tinyobjloader.h"
#include <glm/glm.hpp>

class Model3d {
public:
  Model3d() = default;
  bool load(std::string file);
  void render();

private:
  void createBuffers();

  std::vector<float> vertices_buffer_;

  glm::vec3 getVertex(int index);
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

};