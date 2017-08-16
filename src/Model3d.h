#pragma once

#include "tinyobjloader.h"
#include <glm/glm.hpp>

#include <map>

// It should be instantiated with a Object3dHolder
class Object3d {
public:
  Object3d(const std::string& name) : name_(name) {}
  void setVertices(const std::vector<float>& vertices) { vertices_buffer_ = vertices; }
  void setNormals(const std::vector<float>& normals) { normals_buffer_ = normals; }
  void setColors(const std::vector<float>& colors) { colors_buffer_ = colors; }
  int getNumberTriangles() const { return (int)vertices_buffer_.size() / 3; }
  std::string getName() const { return name_; }

  void render();
private:
  std::vector<float> vertices_buffer_;
  std::vector<float> normals_buffer_;
  std::vector<float> colors_buffer_;
  std::string name_;
};

class Object3dHolder {
public:
  void setRotationZ(float degrees) { rotation_.z = degrees; }
  void setRotation(glm::vec3 degrees) { rotation_ = degrees; }
  void setTranslation(glm::vec3 translation) { translation_ = translation; }
  void setObject3d(Object3d* obj) { object_ = obj;  }
  void setScale(glm::vec3 scale) { scale_ = scale; }
  void render();
private:
  Object3d* object_ = nullptr;
  glm::vec3 rotation_ = glm::vec3(0.f);
  glm::vec3 translation_ = glm::vec3(0.f);
  glm::vec3 scale_ = glm::vec3(1.0);
};

/* 
 * A set of Object3d objects
 */
class Model3d {
public:
  Model3d() = default;
  bool load(std::string file);
  
  // Renders all the object3d
  void render();
  Object3d* getObject3d(const std::string& obj_prefix);

private:
  void createBuffers();
  std::vector<Object3d> objects_;

  glm::vec3 getVertex(tinyobj::index_t);
  glm::vec3 getNormal(tinyobj::index_t);
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

};