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
  void setRotation(float rot_z);
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
