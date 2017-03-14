#pragma once

#include <array>

#include "Math.h"

struct LineSegment {
  glm::vec2 points[2];
};

struct Circle {
  glm::vec2 center{0,0};
  float radius{1.f};
};

class Rectangle {
public:
  Rectangle() = default;
  Rectangle(const glm::vec2& center, const glm::vec2& dim, float rot_z = 0);
  void setRotation(float rot_z);
  void setCenter(const glm::vec2&);
  void setDimensions(const glm::vec2&);
  glm::vec2 getDimensions() const { return dimensions_; }
  float getArea() const;
  const glm::vec2& getCenter() const { return center_; }
  void getVertices(std::array<glm::vec2, 4>* vertices) const;
  void getLineSegments(std::array<LineSegment, 4> *line_segments) const;

private:
  glm::vec2 center_;
  glm::vec2 dimensions_{1,1};
  float rot_z_ = 0.0f;  // degrees
  mutable bool invalidate_ = true;
  mutable std::array<glm::vec2, 4> vertices_;
};

class Geometry {
public:
  static bool isPointInCircle(const glm::vec2&, const Circle&);
  static bool isPointInRectangle(const glm::vec2&, const Rectangle&);
  static glm::vec2 closestPointInLineSegmentToPoint(const LineSegment&, const glm::vec2&);

};