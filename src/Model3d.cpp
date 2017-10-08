#include "Model3d.h"
#include "Logger.h"
#include "Stats.h"

#include "OpenGlResources.h"

void Object3dHolder::render() {
  glPushMatrix();
  glTranslated(translation_.x, translation_.y, translation_.z);

  glScalef(scale_.x, scale_.y, scale_.z);
  glRotatef(rotation_.x, 1, 0, 0);
  glRotatef(rotation_.y, 0, 1, 0);
  glRotatef(rotation_.z, 0, 0, 1);

  object_->render();
  glPopMatrix();
}

// Dont push the matrix!!
void Object3d::render() {
  int num_triangles = (int)vertices_buffer_.size() / 3;
  Stats::getInstance().num_triangles += num_triangles;

  // glPolygonMode(GL_FRONT, GL_LINE);
  // glPolygonMode(GL_BACK, GL_LINE);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);



  // glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  // removed for emscripten... seems to work fine without this
  glEnable(GL_COLOR_MATERIAL);
  glColorPointer(4, GL_FLOAT, 0, colors_buffer_.data());

  glVertexPointer(3, GL_FLOAT, 0, vertices_buffer_.data());
  glNormalPointer(GL_FLOAT, 0, normals_buffer_.data());
  glDrawArrays(GL_TRIANGLES, 0, num_triangles);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisable(GL_COLOR_MATERIAL);
  // glPolygonMode(GL_FRONT, GL_FILL);
  // glPolygonMode(GL_BACK, GL_FILL);
}

bool Model3d::load(std::string file) {
  std::string err;
  
  std::size_t found = file.find_last_of("/");
  std::string parent_path = file.substr(0, found) + "/";
  bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str(), parent_path.c_str());
  if (!result || !err.empty()) {
    return false;
  }
  createBuffers();
  return true;
}

void Model3d::createBuffers() {
  for (auto& shape : shapes) {
    Object3d obj3d(shape.name);
    
    std::vector<float> vertices_buffer;
    std::vector<float> normals_buffer;
    std::vector<float> colors_buffer;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {

      tinyobj::index_t idx;
      glm::vec3 vertex;
      glm::vec3 normal;

      for (int i = 0; i < 3; i++) {
        idx = shape.mesh.indices[3 * f + i];
        vertex = getVertex(idx);
        vertices_buffer.push_back(vertex.x);
        vertices_buffer.push_back(vertex.y);
        vertices_buffer.push_back(vertex.z);

        normal = getNormal(idx);
        normals_buffer.push_back(normal.x);
        normals_buffer.push_back(normal.y);
        normals_buffer.push_back(normal.z);
      }

      int material_id = shape.mesh.material_ids[f];
      for (int repeat = 0; repeat < 3; repeat++) {  // have the color 3 times: 1 per vertex
        for (int i = 0; i < 3; i++) {
          colors_buffer.push_back(materials[material_id].diffuse[i]);
        }
        colors_buffer.push_back(1.f);  // alpha. Have 4 components
      }
    }
    obj3d.setVertices(vertices_buffer);
    obj3d.setNormals(normals_buffer);
    obj3d.setColors(colors_buffer);

    LOG_DEBUG("Object " << obj3d.getName() << " has " << obj3d.getNumberTriangles() << " triangles");
    objects_.push_back(obj3d);
  }
}

glm::vec3 Model3d::getVertex(tinyobj::index_t idx) {
  int index = idx.vertex_index;
  glm::vec3 v(attrib.vertices[index * 3], attrib.vertices[index * 3 + 1], attrib.vertices[index * 3 + 2]);
  return v;
}

glm::vec3 Model3d::getNormal(tinyobj::index_t idx) {
  int index = idx.normal_index;
  glm::vec3 v(attrib.normals[index * 3], attrib.normals[index * 3 + 1], attrib.normals[index * 3 + 2]);
  return v;
}

Object3d* Model3d::getObject3d(const std::string& obj_prefix) {
  assert(!obj_prefix.empty());
  for (auto& obj : objects_) {
    if (obj.getName().find(obj_prefix) == 0) {
      return &obj;
    }
  }
  assert(false);
  return nullptr;
}

// A suitable opengl context is assumed to be initialized
// make this opengl 2.0 with buffers
void Model3d::render() {
  for (auto& obj : objects_) {
    obj.render();
  }
}
