#include "Model3d.h"
#include "utils/Logger.h"
#include "utils/Stats.h"

Object3d::Object3d(const std::string& name) : name_(name) {
  glGenBuffers(1, &ogl_buffer_vertex_attribs_);
}

Object3d::~Object3d() {
  glDeleteBuffers(1, &ogl_buffer_vertex_attribs_);
}

void Object3dHolder::calculateModelMatrix() {
  if (valid_model_mat_) return;

  model_mat_ = glm::translate(translation_);
  model_mat_ *= glm::scale(scale_);
  model_mat_ *= glm::rotate(glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
  model_mat_ = glm::rotate(model_mat_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
  model_mat_ = glm::rotate(model_mat_, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
  valid_model_mat_ = true;
}

void Object3dHolder::render(bool render_shadow) {
  glPushMatrix();
  calculateModelMatrix();
  glMultMatrixf(glm::value_ptr(model_mat_));
  object_->render();
  if (render_shadow) {
    object_->renderVolumeShadow(model_mat_, glm::vec4(20, 20, 20, 1));
  }
  glPopMatrix();
}

void Object3d::renderVolumeShadow(const glm::mat4& model_mat, const glm::vec4& ligth_pos) {
  // By now, model_mat_ has the correct value
  // We are under the "model matrix" (M) influence, so we need to apply the inverse to the light, 
  // so it gets to "light_pos" after we apply M * (M^-1 * light_pos) 
  glm::vec4 new_light_pos =  glm::inverse(model_mat) * ligth_pos;

  // Vertices are ordered by triangles. Vertex shared among triangles will be repeated
  glm::vec3 direction_vector;
  glBegin(GL_QUADS);
  for (int i_vertex = 0; i_vertex < vertices_buffer_.size(); i_vertex += 3) {
    std::array<glm::vec3, 3> original_triangle = {
      vertices_buffer_[i_vertex + 0],
      vertices_buffer_[i_vertex + 1],
      vertices_buffer_[i_vertex + 2]
    };

    std::array<glm::vec3, 3> projection_triangle;
    for (int i = 0; i < 3; i++) {
      glm::vec3 vertex = original_triangle[i];
      direction_vector = new_light_pos.w == 0 ? glm::vec3(new_light_pos) : glm::normalize(vertex - glm::vec3(new_light_pos));
      projection_triangle[i] = vertex + direction_vector * 50.f;
    }

    // Draw the quads
    glm::vec3 vertex;
    for (int i = 0; i < 3; i++) {
      vertex = original_triangle[i];    glVertex3f(vertex.x, vertex.y, vertex.z);
      vertex = projection_triangle[i];  glVertex3f(vertex.x, vertex.y, vertex.z);
      vertex = projection_triangle[(i + 1) % 3];  glVertex3f(vertex.x, vertex.y, vertex.z);
      vertex = original_triangle[(i + 1) % 3];    glVertex3f(vertex.x, vertex.y, vertex.z);
    }   
  }
  glEnd();
}

// Dont push the matrix!!
void Object3d::render() {
  int num_triangles = getNumberTriangles();
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
  glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisable(GL_COLOR_MATERIAL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Object3d::setData(const std::vector<glm::vec3>& vertices,
                       const std::vector<glm::vec3>& normals,
                       const std::vector<glm::vec3>& colors) {
  assert(vertices.size() == normals.size());
  assert(vertices.size() == colors.size());
  assert(vertices.size() % 3 == 0);
  
  std::vector<float> vertex_attributes;  // coordinates0, normals0, colors0, coordinates1, ...

  for (int i = 0; i < vertices.size(); i++) {
    vertex_attributes.push_back(vertices[i].x);
    vertex_attributes.push_back(vertices[i].y);
    vertex_attributes.push_back(vertices[i].z);

    vertex_attributes.push_back(normals[i].x);
    vertex_attributes.push_back(normals[i].y);
    vertex_attributes.push_back(normals[i].z);

    vertex_attributes.push_back(colors[i].x);
    vertex_attributes.push_back(colors[i].y);
    vertex_attributes.push_back(colors[i].z);
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
    
    std::vector<glm::vec3> vertices_buffer;
    std::vector<glm::vec3> normals_buffer;
    std::vector<glm::vec3> colors_buffer;

    for (size_t f = 0; f < shape.mesh.indices.size() / 3; f++) {
      tinyobj::index_t idx;
      glm::vec3 vertex;
      glm::vec3 normal;

      for (int i = 0; i < 3; i++) {
        idx = shape.mesh.indices[3 * f + i];
        vertex = getVertex(idx);
        vertices_buffer.push_back(vertex);

        normal = getNormal(idx);
        normals_buffer.push_back(normal);
      }

      int material_id = shape.mesh.material_ids[f];
      for (int repeat = 0; repeat < 3; repeat++) {  // have the color 3 times: 1 per vertex
        glm::vec3 color;
        color.x = materials[material_id].diffuse[0];
        color.y = materials[material_id].diffuse[1];
        color.z = materials[material_id].diffuse[2];

        colors_buffer.push_back(color);
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
