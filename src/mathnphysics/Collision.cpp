#include "Collision.h"

#include <map>

#include "graphics/OpenGlResources.h"
#include "graphics/OpenGlState.h"

void CollisionArea::setPosition(const glm::vec2& pos) {
  switch (type_) {
  case CollisionAreaType::Circle:
    circle.center = pos;
    break;
  case CollisionAreaType::Rectangle:
    rectangle.setCenter(pos);
    break;
  }
}

glm::vec2 CollisionArea::getPosition() const {
  switch (type_) {
  case CollisionAreaType::Circle:
    return circle.center;
    break;
  case CollisionAreaType::Rectangle:
    return rectangle.getCenter();
    break;
  }
  assert(false);
  return glm::vec2(0, 0);
}

void CollisionArea::setRotation(float rot_z) {
  switch (type_) {
  case CollisionAreaType::Rectangle:
    rectangle.setRotation(rot_z);
    break;
  }
}


void DrawCircle(const Circle& c, int num_segments) {
  glm::mat4 translation_m = glm::translate(glm::vec3(c.center.x, c.center.y, 0));
  OpenGlState::getInstance().multMatrix(translation_m);
  OpenGlResources::drawCircle(c.radius, 10);
}

void DrawRectangle(const Rectangle& r) {
  std::array<glm::vec2, 4> vertices;
  r.getVertices(&vertices);
  std::vector<glm::vec2> vertices_vector{ vertices.begin(), vertices.end() };
  OpenGlResources::drawPolygon(vertices_vector);
}

void CollisionArea::setCollisionPrimivite(const Rectangle& rect) {
  type_ = CollisionAreaType::Rectangle;
  rectangle = rect;
}

void CollisionArea::setCollisionPrimivite(const Circle& c) {
  type_ = CollisionAreaType::Circle;
  circle = c;
}


void CollisionArea::render() const {
  OpenGlState::getInstance().pushMatrix();
  switch (type_) {
  case CollisionAreaType::Circle:
    DrawCircle(circle, 10);
    break;
  case CollisionAreaType::Rectangle:
    DrawRectangle(rectangle);
    break;
  }
  OpenGlState::getInstance().popMatrix();
}

bool Collision::isCollision(const CollisionArea& area1, const CollisionArea& area2, glm::vec2* collision_point) {
  if (area1.getType() == CollisionAreaType::None || area2.getType() == CollisionAreaType::None) {
    return false;
  }

  if (area1.getType() == CollisionAreaType::Circle && area2.getType() == CollisionAreaType::Circle) {
    return isCollision(area1.circle, area2.circle, collision_point);
  }
  if (area1.getType() == CollisionAreaType::Circle && area2.getType() == CollisionAreaType::Rectangle) {
    return isCollision(area1.circle, area2.rectangle, collision_point);
  }
  if (area1.getType() == CollisionAreaType::Rectangle && area2.getType() == CollisionAreaType::Circle) {
    return isCollision(area2.circle, area1.rectangle, collision_point);
  }
  if (area1.getType() == CollisionAreaType::Rectangle && area2.getType() == CollisionAreaType::Rectangle) {
    return isCollision(area1.rectangle, area2.rectangle, collision_point);
  }
  assert(false && "Not implemented yet");
  return false;
}


bool Collision::isCollision(const Circle& c1, const Circle& c2, glm::vec2* collision_point) {
  float distance_squared = glm::distance2(c1.center, c2.center);
  float radius_sum = c1.radius + c2.radius;
  bool is_collision = distance_squared < radius_sum * radius_sum;
  if (is_collision) {
    *collision_point = (c2.center - c1.center) * c1.radius / radius_sum + c1.center;
  }
  return is_collision;
}

// Collision point: approximation
bool Collision::isCollision(const Circle& c, const Rectangle& r, glm::vec2* collision_point) {
  std::array<LineSegment, 4> line_segments;
  r.getLineSegments(&line_segments);
  bool is_collision =
    Geometry::isPointInRectangle(c.center, r) ||
    Geometry::isPointInCircle(r.getCenter(), c) ||
    Geometry::isSegmentInCircle(line_segments[0], c) ||
    Geometry::isSegmentInCircle(line_segments[1], c) ||
    Geometry::isSegmentInCircle(line_segments[2], c) ||
    Geometry::isSegmentInCircle(line_segments[3], c);

  if (is_collision) {
    std::map<float, glm::vec2> distance_point_map;  // use map to order
    for (auto& line_segment : line_segments) {
      glm::vec2 point = Geometry::closestPointInLineSegmentToPoint(line_segment, c.center);
      distance_point_map[glm::length(point-c.center)] = point;
    }
    *collision_point = distance_point_map.begin()->second;
  }
  return is_collision;
    
}

bool Collision::isCollision(const Rectangle& r1, const Rectangle& r2, glm::vec2* collision_point) {
  return false;
}
