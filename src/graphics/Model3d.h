#pragma once

#include "tinyobjloader.h"
#include "mathnphysics/Math.h"
#include "OpenGlResources.h"

#include <map>
#include <memory>

// It should be instantiated with a Object3dHolder
class Object3d {
public:
  Object3d(const std::string& name);
  ~Object3d();
  // color: 3 components
  void setData(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& colors);
  int getNumberTriangles() const { return (int)vertices_buffer_.size() / 3; }
  std::string getName() const { return name_; }

  void render();
  void renderVolumeShadow(const glm::mat4& model, const glm::vec4& light_pos);

private:
  std::vector<glm::vec3> vertices_buffer_;
  std::vector<glm::vec3> normals_buffer_;
  std::vector<glm::vec3> colors_buffer_;
  std::string name_;
  GLuint ogl_buffer_vertex_attribs_ = 0;
};

class Object3dHolder {
public:
  void setRotationZ(float degrees) { 
    rotation_.z = degrees;
    valid_model_mat_ = false;
  }
  void setRotation(glm::vec3 degrees) { 
    rotation_ = degrees;
    valid_model_mat_ = false;
  }
  void setTranslation(glm::vec3 translation) { 
    translation_ = translation; 
    valid_model_mat_ = false;
  }
  void setObject3d(Object3d* obj) { object_ = obj;  }
  void setOpenGlProgram(OpenGlProgram* ogl_program) { ogl_program_ = ogl_program; }
  void setScale(glm::vec3 scale) { 
    scale_ = scale;
    valid_model_mat_ = false;
  }
  void render(bool render_shadow);
private:
  void calculateModelMatrix();

  Object3d* object_ = nullptr;
  glm::vec3 rotation_ = glm::vec3(0.f);
  glm::vec3 translation_ = glm::vec3(0.f);
  glm::vec3 scale_ = glm::vec3(1.0);
  glm::mat4 model_mat_;
  bool valid_model_mat_ = false;
  OpenGlProgram* ogl_program_ = nullptr;
};

/* 
 * A set of Object3d objects: the blender model can contain a subset of "Object3d"
 */
class Model3d {
public:
  Model3d() = default;
  bool load(std::string file);
  
  Object3d* getObject3d(const std::string& obj_prefix);

private:
  void createBuffers();
  std::vector<std::shared_ptr<Object3d>> objects_;

  glm::vec3 getVertex(tinyobj::index_t);
  glm::vec3 getNormal(tinyobj::index_t);
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

};
