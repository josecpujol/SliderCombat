#pragma once

#include <deque>

#include "SDL.h"
#include "Math.h"
#include "Collision.h"

enum class GameObjectType {Undefined, Projectile, LocalPlayer, ComputerEnemy, Explosion, HealthPowerup};

struct Pose {
  Pose(const glm::vec3& pos, float rot) : position(pos), rotation(rot) {}
  Pose() = delete;
  glm::vec3 position;
  float rotation;
};

class GameObject {
public:
  GameObject() = delete;
  GameObject(GameObjectType type, glm::vec3 pos, float rot, float mass) : type_(type), mass_(mass) {
    setPose(pos, rot);
  }

  GameObject(const GameObject&) = delete;
  GameObject(const GameObject& obj, GameObjectType type) {
    this->type_ = type;
    this->pos_ = obj.pos_;
    this->rot_z_ = obj.rot_z_;
    // dont copy collision area
  }

  virtual ~GameObject() {}

  virtual void render() = 0;
  void renderCollisionArea();
  virtual void update(const Uint8* keys, uint32_t elapsed_us) = 0;
  virtual void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) = 0;
  GameObjectType getType() const { return type_;  }
  float getMass() const { return mass_; }
  bool canCollide() const { return can_collide_; }

  const glm::vec3& getPosition() const { return pos_; }
  const glm::vec3& getDisplacement() const { return displacement_; }
  float getRotation() const { return rot_z_; }
  const CollisionArea& getCollisionArea() const;
  void setCollisionArea(const CollisionArea& ca);
  void unsetCollisionArea();

  void undoPose();

protected:
  void setPose(glm::vec3 pos, float rot);
  void setPose(glm::vec3 pos);
  
private:
  float mass_ = 0.f;
  glm::vec3 displacement_;
  float rot_z_ = 0.0;  // degrees
  glm::vec3 pos_ = glm::vec3(0.0, 0.0, 0.1);
  GameObjectType type_ = GameObjectType::Undefined;
  bool can_collide_ = false;
  CollisionArea collision_area_;
  std::deque<Pose> poses_;
  size_t max_poses_ = 5;
};
