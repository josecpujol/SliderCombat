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