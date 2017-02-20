#include "Collision.h"

#include "OpenGlResources.h"

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

void CollisionArea::setRotation(float rot_z) {
  switch (type_) {
  case CollisionAreaType::Rectangle:
    rectangle.setRotation(rot_z);
    break;
  }
}


void DrawCircle(const Circle& c, int num_segments) {
  glTranslatef(c.center.x, c.center.y, 0);
  OpenGlResources::drawCircle(c.radius, 10);
}

void DrawRectangle(const Rectangle& r) {
  std::array<glm::vec2, 4> vertices;
  r.getVertices(vertices);
  glBegin(GL_LINE_LOOP);
  for (auto& v : vertices) {
    glVertex2d(v.x, v.y);
  }
  glEnd();
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
  glPushMatrix();
  switch (type_) {
  case CollisionAreaType::Circle:
    DrawCircle(circle, 10);
    break;
  case CollisionAreaType::Rectangle:
    DrawRectangle(rectangle);
    break;
  }
  glPopMatrix();
}

bool Collision::isCollision(const CollisionArea& area1, const CollisionArea& area2) {
  if (area1.getType() == CollisionAreaType::None || area2.getType() == CollisionAreaType::None) {
    return false;
  }

  if (area1.getType() == CollisionAreaType::Circle && area2.getType() == CollisionAreaType::Circle) {
    return isCollision(area1.circle, area2.circle);
  }
  if (area1.getType() == CollisionAreaType::Circle && area2.getType() == CollisionAreaType::Rectangle) {
    // Undo rotation of rectangle over circle
    return isCollision(area1.circle, area2.rectangle);
  }
  if (area1.getType() == CollisionAreaType::Rectangle && area2.getType() == CollisionAreaType::Circle) {
    return isCollision(area2.circle, area1.rectangle);
  }
  if (area1.getType() == CollisionAreaType::Rectangle && area2.getType() == CollisionAreaType::Rectangle) {
    return isCollision(area1.rectangle, area2.rectangle);
  }
  assert(false && "Not implemented yet");
  return false;
}


bool Collision::isCollision(const Circle& c1, const Circle& c2) {
  float distance_squared = glm::distance2(c1.center, c2.center);
  float radius_sum = c1.radius + c2.radius;
  return (distance_squared < radius_sum * radius_sum);
}

bool Collision::isCollision(const Circle& c, const Rectangle& r) {
  return
    Geometry::isPointInRectangle(c.center, r) ||
    Geometry::isPointInCircle(r.getCenter(), c);
// TODO: add intersection of segments with circle
}

bool Collision::isCollision(const Rectangle& r1, const Rectangle& r2) {
  return false;
}