#include "Model3d.h"
#include "Logger.h"
#include <GL/glew.h>
bool Model3d::load(std::string file) {
  std::string err;

  bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str());
  if (!result) {
    return false;
  }
  createBuffers();
  return true;
}

void Model3d::createBuffers() {
  for (auto& shape : shapes) {
    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
      tinyobj::index_t idx;
      glm::vec3 vertex;

      for (int i = 0; i < 3; i++) {
        idx = shape.mesh.indices[3 * f + i];
        vertex = getVertex(idx.vertex_index);
        vertices_buffer_.push_back(vertex.x);
        vertices_buffer_.push_back(vertex.y);
        vertices_buffer_.push_back(vertex.z);
      }
    }
  }
}

glm::vec3 Model3d::getVertex(int index) {
  glm::vec3 v(attrib.vertices[index * 3], attrib.vertices[index * 3 + 1], attrib.vertices[index * 3 + 2]);
  return v;
}

// A suitable opengl context is assumed to be initialized
// make this opengl 2.0 with buffers
void Model3d::render() {
  glColor3f(1, 1, 1);
  glPolygonMode(GL_FRONT, GL_LINE);
  glPolygonMode(GL_BACK, GL_LINE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_buffer_.data());
  glDrawArrays(GL_TRIANGLES, 0, (int)(vertices_buffer_.size() / 3));
  glDisableClientState(GL_VERTEX_ARRAY);
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

}