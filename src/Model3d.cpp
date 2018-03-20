#include "Model3d.h"
#include "Logger.h"
#include "Stats.h"

Object3d::Object3d(const std::string& name) : name_(name) {
  glGenBuffers(1, &ogl_buffer_vertex_attribs_);

}

Object3d::~Object3d() {
  glDeleteBuffers(1, &ogl_buffer_vertex_attribs_);
}

void Object3dHolder::calculateModelMatrix() {
  if (!invalidate_model_mat_) return;

  model_mat_ = glm::translate(translation_);
  model_mat_ = glm::scale(model_mat_, scale_);
  model_mat_ = glm::rotate(model_mat_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
  model_mat_ = glm::rotate(model_mat_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
  model_mat_ = glm::rotate(model_mat_, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
  invalidate_model_mat_ = false;
}

void Object3dHolder::render(bool render_shadow) {
  glPushMatrix();
  calculateModelMatrix();
  glMultMatrixf(glm::value_ptr(model_mat_));
  object_->render();
  if (render_shadow) {
  //  object_->renderVolumeShadow(model_mat_, glm::vec4(0, 0, 20, 1));
  }
  glPopMatrix();
}

void Object3d::renderVolumeShadow(const glm::mat4& model_mat, const glm::vec4& ligth_pos) {
  // By now, model_mat_ has the correct value
  glm::vec4 new_light_pos =  glm::inverse(model_mat) * ligth_pos;

  glm::vec3 direction_vector;
  glBegin(GL_LINES);
  for (int i = 0; i < vertices_buffer_.size(); i+=3) {
    glm::vec3 vertex(vertices_buffer_[i], vertices_buffer_[i + 1], vertices_buffer_[i + 2]);
    direction_vector = new_light_pos.w == 0 ? new_light_pos : glm::normalize(vertex - glm::vec3(new_light_pos));
    glVertex3f(vertex.x, vertex.y, vertex.z);
    glm::vec3 dest = vertex + direction_vector * 5.f;
    glVertex3f(dest.x, dest.y, dest.z);
  }
  glEnd();
}

// Dont push the matrix!!
void Object3d::render() {
  int num_triangles = (int)vertices_buffer_.size() / 3;
  Stats::getInstance().num_objects++;
  Stats::getInstance().num_triangles += num_triangles;

  glEnable(GL_COLOR_MATERIAL);

  glBindBuffer(GL_ARRAY_BUFFER, ogl_buffer_vertex_attribs_);
  GLsizei stride = 9 * sizeof(float);
  glVertexPointer(3, GL_FLOAT, stride, (void*)0);
  glNormalPointer(GL_FLOAT, stride, (void*)(3 * sizeof(float)));
  glColorPointer(3, GL_FLOAT, stride, (void*)(6 * sizeof(float)));

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, num_triangles);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisable(GL_COLOR_MATERIAL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Object3d::setData(const std::vector<float>& vertices,
                       const std::vector<float>& normals, 
                       const std::vector<float>& colors) {
  assert(vertices.size() == normals.size());
  assert(vertices.size() == colors.size());
  assert(vertices.size() % 3 == 0);
  
  std::vector<float> vertex_attributes;  // coordinates0, normals0, colors0, coordinates1, ...

  for (int i = 0; i < vertices.size() / 3; i++) {
    vertex_attributes.push_back(vertices[i * 3 + 0]);
    vertex_attributes.push_back(vertices[i * 3 + 1]);
    vertex_attributes.push_back(vertices[i * 3 + 2]);

    vertex_attributes.push_back(normals[i * 3 + 0]);
    vertex_attributes.push_back(normals[i * 3 + 1]);
    vertex_attributes.push_back(normals[i * 3 + 2]);

    vertex_attributes.push_back(colors[i * 3 + 0]);
    vertex_attributes.push_back(colors[i * 3 + 1]);
    vertex_attributes.push_back(colors[i * 3 + 2]);
  }
  
  glBindBuffer(GL_ARRAY_BUFFER, ogl_buffer_vertex_attribs_);
  glBufferData(GL_ARRAY_BUFFER, vertex_attributes.size() * sizeof(float), vertex_attributes.data(), GL_STATIC_DRAW);

  vertices_buffer_ = vertices; 
  normals_buffer_ = normals;
  colors_buffer_ = colors;
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
    std::shared_ptr<Object3d> obj3d = std::make_shared<Object3d>(shape.name);
    
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
      }
    }
    obj3d->setData(vertices_buffer, normals_buffer, colors_buffer);

    LOG_DEBUG("Object " << obj3d->getName() << " has " << obj3d->getNumberTriangles() << " triangles");
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
    if (obj->getName().find(obj_prefix) == 0) {
      return obj.get();
    }
  }
  assert(false);
  return nullptr;
}

// A suitable opengl context is assumed to be initialized
// make this opengl 2.0 with buffers
void Model3d::render() {
  for (auto& obj : objects_) {
    obj->render();
  }
}
