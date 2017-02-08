#include "Collision.h"

void CollisionArea::setPosition(const glm::vec2& pos) {
  switch (type) {
  case CollisionAreaType::Circle:
    circle.center = pos;
    break;
  case CollisionAreaType::Rectangle:
    rentangle.origin = pos;
    break;
  }
}

bool Collision::isCollision(const CollisionArea& area1, const CollisionArea& area2) {
  if (area1.type == CollisionAreaType::Circle && area2.type == CollisionAreaType::Circle) {
    return isCollision(area1.circle, area2.circle);
  }
  if (area1.type == CollisionAreaType::Circle && area2.type == CollisionAreaType::Rectangle) {
    // Undo rotation of rectangle over circle
    return isCollision(area1.circle, area2.rentangle);
  }
  if (area1.type == CollisionAreaType::Rectangle && area2.type == CollisionAreaType::Circle) {
    return isCollision(area2.circle, area1.rentangle);
  }
  assert(false && "Not implemented yet");
}


bool Collision::isCollision(const Circle& c1, const Circle& c2) {
  float distance_squared = glm::distance2(c1.center, c2.center);
  float radius_sum = c1.radius + c2.radius;
  return (distance_squared < radius_sum * radius_sum);
}

bool Collision::isCollision(const Circle& c, const Rectangle& r) {
  return true;
}

bool Collision::isCollision(const Rectangle& r1, const Rectangle& r2) {
  return true;
}