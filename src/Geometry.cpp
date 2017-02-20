#include "Geometry.h"

Rectangle::Rectangle(const glm::vec2& center, const glm::vec2& dim, float rot_z) {
  setRotation(rot_z);
  setCenter(center);
  setDimensions(dim);
}

void Rectangle::setRotation(float rot_z) {
  rot_z_ = rot_z;
  invalidate_ = true;
}

void Rectangle::setCenter(const glm::vec2& center) {
  center_ = center;
  invalidate_ = true;
}
void Rectangle::setDimensions(const glm::vec2& dim) {
  dimensions_ = dim;
  invalidate_ = true;
}

float Rectangle::getArea() const {
  return dimensions_.x * dimensions_.y;
}

void Rectangle::getVertices(std::array<glm::vec2, 4>& vertices) const {
  if (invalidate_) {
    vertices_[0] = glm::vec2(-dimensions_.x, -dimensions_.y);
    vertices_[1] = glm::vec2(dimensions_.x, -dimensions_.y);
    vertices_[2] = glm::vec2(dimensions_.x, dimensions_.y);
    vertices_[3] = glm::vec2(-dimensions_.x, dimensions_.y);

    // *0.5 (half dimensions), rotate and translate
    for (auto& v : vertices_) {
      v *= 0.5f;
      v = applyRotation(v, rot_z_);
      v += center_;
    }
    invalidate_ = false;
  }
  vertices = vertices_;
}

bool Geometry::isPointInCircle(const glm::vec2& p, const Circle& c) {
  return glm::distance2(p, c.center) < (c.radius * c.radius);
}

float areaTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
  glm::vec2 p1n = p1 - p3;
  glm::vec2 p2n = p2 - p3;
  return fabsf(p1n.x * p2n.y - p1n.y * p2n.x) / 2;
}

bool Geometry::isPointInRectangle(const glm::vec2& p, const Rectangle& r) {
  std::array< glm::vec2, 4> vertices;
  r.getVertices(vertices);
  glm::vec2 dim = r.getDimensions();
  glm::vec2 probe_v = p - vertices[0];
  float projx = (glm::dot(probe_v, vertices[1] - vertices[0]));
  float projy = (glm::dot(probe_v, vertices[3] - vertices[0]));
  return (projx >= 0 && projx <= dim.x * dim.x &&
    projy >= 0 && projy <= dim.y * dim.y);
}
