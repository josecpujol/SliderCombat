#include "Collision.h"

#include <GL/glew.h>

void CollisionArea::setPosition(const glm::vec2& pos) {
  switch (type_) {
  case CollisionAreaType::Circle:
    circle.center = pos;
    break;
  case CollisionAreaType::Rectangle:
    rectangle.origin = pos;
    break;
  }
}


void DrawCircle(const Circle& c, int num_segments) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < num_segments; i++) {
    float theta = (2.0f * 3.1415926f * i) / num_segments;

    float x = c.radius * cosf(theta);
    float y = c.radius * sinf(theta);

    glVertex2f(x + c.center.x, y + c.center.y);//output vertex

  }
  glEnd();
}

void DrawRectangle(const Rectangle& r) {
  glTranslatef(r.origin.x, r.origin.y, 0);
  glRotatef(r.rot_z, 0, 0, 1);
  glBegin(GL_LINE_LOOP);
  glVertex2d(0, 0);
  glVertex2d(0, r.dimensions.y);
  glVertex2d(r.dimensions.x, r.dimensions.y);
  glVertex2d(r.dimensions.x, 0);
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
  assert(false && "Not implemented yet");
  return false;
}


bool Collision::isCollision(const Circle& c1, const Circle& c2) {
  float distance_squared = glm::distance2(c1.center, c2.center);
  float radius_sum = c1.radius + c2.radius;
  return (distance_squared < radius_sum * radius_sum);
}

bool Collision::isCollision(const Circle& c, const Rectangle& r) {
  return false;
}

bool Collision::isCollision(const Rectangle& r1, const Rectangle& r2) {
  return false;
}