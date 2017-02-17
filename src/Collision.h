#pragma once

#include "Math.h"

struct Circle {
  glm::vec2 center{0,0};
  float radius{1.f};
};

struct Rectangle {
  glm::vec2 origin;
  glm::vec2 dimensions{1,1};
  float rot_z = 0.0f;  // degrees
};

enum class CollisionAreaType { None, Circle, Rectangle };

class CollisionArea {
public:
  Circle circle;
  Rectangle rectangle;
  CollisionAreaType getType() const { return type_; }
  void setCollisionPrimivite(const Rectangle& rect);
  void setCollisionPrimivite(const Circle& c);
  void setPosition(const glm::vec2& pos);
  void render() const;  // for debugging purposes
private:
  CollisionAreaType type_ = CollisionAreaType::None;
};

class Collision {
public:
  Collision() = default;
  static bool isCollision(const CollisionArea& area1, const CollisionArea& area2);
  
private:
  static bool isCollision(const Circle&, const Circle&);
  static bool isCollision(const Circle&, const Rectangle&);
  static bool isCollision(const Rectangle&, const Rectangle&);
};
