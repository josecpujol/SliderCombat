#pragma once

#include "GameObject.h"
#include "graphics/Model3d.h"
#include "ResourcesManager.h"
#include "utils/Time.h"

class PowerUp : public GameObject {
public:
  PowerUp() = delete;
  PowerUp(GameObjectType type, const glm::vec2& pos);
  void update(uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override;

private:
  void enable();
  void disableFor(Duration);
  Object3dHolder base_model_;
  Object3dHolder powerup_model_;
  float rotation_ = 0.f;
  float initial_height_ = 1.0f;
  uint32_t time_elapsed_us_ = 0;
  bool is_visible_ = true;
  Duration enable_cooldown_ = 0s;
};

class HealthPowerUp : public PowerUp {
public:
  HealthPowerUp(const glm::vec2& pos) : PowerUp(GameObjectType::HealthPowerup, pos) {};
};