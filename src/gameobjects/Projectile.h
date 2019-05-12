#pragma once

#include "SDL.h"

#include "GameObject.h"
#include "ResourcesManager.h"

class Projectile : public GameObject {
public:
  Projectile(glm::vec3 pos, float rot);
  ~Projectile() {}
  void update(uint32_t elapsed_us) override;
  void render(Scene&) override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override;
  int getDamage() { return damage_; }
  float getVelocity() const { return vel_; }


private:
  // m/s
  float vel_ = 20.0;
  int damage_ = 5;
  Object3dHolder model_;
};