#pragma once

#include "Math.h"

struct Circle {
  glm::vec2 center{0,0};
  float radius{1.f};
};

struct Rectangle {
  glm::vec2 origin;
  glm::vec2 dimensions{1,1};
  float rot_z;  // radians
};

enum class CollisionAreaType { Circle, Rectangle };

struct CollisionArea {
  CollisionAreaType type;
  Circle circle;
  Rectangle rentangle;
  void setPosition(const glm::vec2& pos);
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
