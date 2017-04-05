#pragma once

#include "Math.h"
#include "Geometry.h"

enum class CollisionAreaType { None, Circle, Rectangle };

class CollisionArea {
public:
  CollisionArea() = default;
  CollisionArea(const Circle& c) {
    setCollisionPrimivite(c);
  };
  CollisionArea(const Rectangle& r) {
    setCollisionPrimivite(r);
  }

  Circle circle;
  Rectangle rectangle;
  CollisionAreaType getType() const { return type_; }
  void setCollisionPrimivite(const Rectangle& rect);
  void setCollisionPrimivite(const Circle& c);
  void setPosition(const glm::vec2& pos);
  glm::vec2 getPosition() const;
  void setRotation(float rot_z);
  void render() const;  // for debugging purposes
private:
  CollisionAreaType type_ = CollisionAreaType::None;
};

class Collision {
public:
  Collision() = default;
  static bool isCollision(const CollisionArea& area1, const CollisionArea& area2, BoundVector2* collision_vector);
  
private:
  static bool isCollision(const Circle&, const Circle&, glm::vec2* collision_point);
  static bool isCollision(const Circle&, const Rectangle&, glm::vec2* collision_point);
  static bool isCollision(const Rectangle&, const Rectangle&, glm::vec2* collision_point);
};
