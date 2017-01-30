#pragma once

#include "tinyobjloader.h"
#include <glm/glm.hpp>

#include <map>

// It should be instantiated with a Object3dHolder
class Object3d {
public:
  std::vector<float> vertices_buffer;
  std::vector<float> colors_buffer;
  std::string name;

  void render();
};

class Object3dHolder {
public:
  void setRotationZ(float degrees) { rotation_z_ = degrees; }
  void setTranslation(glm::vec3 translation) { translation_ = translation; }
  void setObject3d(Object3d* obj) { object_ = obj;  }
  void setScale(float scale) { scale_ = scale; }
  void render();
private:
  Object3d* object_ = nullptr;
  float rotation_z_ = 0;
  glm::vec3 translation_;
  float scale_ = 1.0;
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
  Object3d* getObject3d(std::string obj_prefix);

private:
  void createBuffers();
  std::vector<Object3d> objects_;

  glm::vec3 getVertex(int index);
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

};