#pragma once

#include "SDL.h"
#include "Math.h"
#include "Collision.h"

enum class GameObjectType {Undefined, Fire, LocalPlayer, ComputerEnemy};

class GameObject {
public:
  GameObject() = delete;
  GameObject(GameObjectType type, glm::vec3 pos, float rot) : type_(type) {
    Circle circle;
    circle.radius = 1.0;
    collision_area_.setCollisionPrimivite(circle);
    setPosition(pos);
    setRotation(rot);
  }

  GameObject(const GameObject&) = delete;
  GameObject(const GameObject& obj, GameObjectType type) {
    this->type_ = type;
    this->pos_ = obj.pos_;
    this->rot_z_ = obj.rot_z_;
    // dont copy collision area
  }
  virtual void render() = 0;
  void renderCollisionArea();
  virtual void update(const Uint8* keys, uint32_t elapsed_us) = 0;
  virtual void onCollision(GameObject* with) = 0;
  GameObjectType getType() const { return type_;  }
  virtual ~GameObject() {}

  const glm::vec3& getPosition() { return pos_; }
  float getRotation() const { return rot_z_; }
  const CollisionArea& getCollisionArea() const { return collision_area_; }
  void setCollisionArea(const CollisionArea& ca);

protected:
  float rot_z_ = 0.0;  // degrees
  void setPosition(const glm::vec3& pos);
  void setRotation(float rot);

private:

  glm::vec3 pos_ = glm::vec3(0.0, 0.0, 0.1);
  GameObjectType type_ = GameObjectType::Undefined;
  CollisionArea collision_area_;
};