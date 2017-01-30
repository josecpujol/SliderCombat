#include "Model3d.h"
#include "Logger.h"
#include <GL/glew.h>
#include <experimental/filesystem>

void Object3dHolder::render() {
  glPushMatrix();
  glTranslated(translation_.x, translation_.y, translation_.z);
  glScalef(scale_.x, scale_.y, scale_.z);
  glRotatef(rotation_z_, 0, 0, 1);
  object_->render();
  glPopMatrix();
}


// Dont push the matrix!!
void Object3d::render() {
  int num_triangles = (int)vertices_buffer.size() / 3;
  // glPolygonMode(GL_FRONT, GL_LINE);
  // glPolygonMode(GL_BACK, GL_LINE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices_buffer.data());
  glColorPointer(3, GL_FLOAT, 0, colors_buffer.data());
  glDrawArrays(GL_TRIANGLES, 0, num_triangles);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  // glPolygonMode(GL_FRONT, GL_FILL);
  // glPolygonMode(GL_BACK, GL_FILL);
}

bool Model3d::load(std::string file) {
  std::string err;
  
  std::experimental::filesystem::path mypath(file);
  std::string parent_path = mypath.parent_path().string() + "/";
  bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str(), parent_path.c_str());
  if (!result || !err.empty()) {
    return false;
  }
  createBuffers();
  return true;
}

void Model3d::createBuffers() {
  for (auto& shape : shapes) {
    Object3d obj3d;
    obj3d.name = shape.name;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {

      tinyobj::index_t idx;
      glm::vec3 vertex;

      for (int i = 0; i < 3; i++) {
        idx = shape.mesh.indices[3 * f + i];
        vertex = getVertex(idx.vertex_index);
        obj3d.vertices_buffer.push_back(vertex.x);
        obj3d.vertices_buffer.push_back(vertex.y);
        obj3d.vertices_buffer.push_back(vertex.z);
      }

      int material_id = shape.mesh.material_ids[f];
      for (int repeat = 0; repeat < 3; repeat++) {  // have the color 3 times: 1 per vertex
        for (int i = 0; i < 3; i++) {
          obj3d.colors_buffer.push_back(materials[material_id].diffuse[i]);
        }
      }
    }
    objects_.push_back(obj3d);
  }
}

glm::vec3 Model3d::getVertex(int index) {
  glm::vec3 v(attrib.vertices[index * 3], attrib.vertices[index * 3 + 1], attrib.vertices[index * 3 + 2]);
  return v;
}

Object3d* Model3d::getObject3d(std::string obj_prefix) {
  assert(!obj_prefix.empty());
  for (auto& obj : objects_) {
    if (obj.name.find(obj_prefix) == 0) {
      return &obj;
    }
  }
  return nullptr;
}

// A suitable opengl context is assumed to be initialized
// make this opengl 2.0 with buffers
void Model3d::render() {
  for (auto& obj : objects_) {
    obj.render();
  }
}