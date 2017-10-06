#pragma once

#include "SDL.h"

#include <vector>

#include "GameObject.h"
#include "Time.h"
#include "Math.h"
#include "ResourcesManager.h"
#include "Physics.h"

class Projectile;

class Meter {
public:
  Meter(int value, int max_value) : value_(value), max_value_(max_value) {};
  void addValue(int inc) {
    value_ += inc;
    if (value_ < 0) value_ = 0;
    if (value_ > max_value_) value_ = max_value_;
  }
  int getValue() const { return value_; }
  int getMaxValue() const { return max_value_; }
private:
  int value_ = 0;
  int max_value_ = 100;
};

class Propeller {
public:
  Propeller(glm::vec2 location, glm::vec2 direction, Uint8 direct_key, Uint8 reverse_key) : 
    force_(BoundVector2(location, glm::normalize(direction))), direct_key_(direct_key), reverse_key_(reverse_key) {
  }

  void update(const Uint8* keys) {
    if (keys[direct_key_]) direct();
    else if (keys[reverse_key_]) reverse();
    else neutral();
  }

  glm::vec2 getForceVector() const {
    return force_magnitude_ * force_.getForceFreeVector();
  }

  // Assuming CoM to be in 0,0
  float getTorque() const {
    return force_magnitude_ * force_.getTorque();
  }

private:
  void direct() { force_magnitude_ = force_max_; }
  void reverse() { force_magnitude_ = -force_max_; }
  void neutral() { force_magnitude_ = 0.f; }
  float force_magnitude_ = 0.0f;
  float force_max_ = 100.f;
  Force force_;
  Uint8 direct_key_;
  Uint8 reverse_key_;
};

class Slider : public GameObject {
public:
  Slider() = delete;
  Slider(GameObjectType type, const glm::vec2& pos, float rot) : 
    GameObject(type, glm::vec3(pos, 0.1), rot, 10.f) {
 //   Rectangle rect(pos, glm::vec2(2, 2), rot);
 //   setCollisionArea(CollisionArea(rect)); 
    Circle circle(pos, 1);
    setCollisionArea(CollisionArea(circle));
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override;
  bool canShoot() const;

  Meter getHealth() const { return health_; }

protected:

  void applyForceAndTorque(const float torque, glm::vec2 force, float elapsed_secs);
  void shoot();

  glm::vec2 global_speed_;  // global coordinates
  float angular_speed_ = 0.f;
  const Duration shot_cooldown_duration_ = 200ms;
  Duration shot_current_cooldown_{0s};

  Duration hit_duration_{0s};

  Meter health_ = Meter(100, 100);
  Model3d* model_ = nullptr;
  Duration time_hit_duration_;
  bool is_hit_ = false;

  std::vector<Force> impacts_;  // local coordinates

  Propeller propellers_[4] = {
    {glm::vec2(-0.5, 0), glm::vec2(0, 1), SDL_SCANCODE_W, SDL_SCANCODE_S},
    {glm::vec2(-1, 0), glm::vec2(-1, 0), SDL_SCANCODE_A, SDL_SCANCODE_D},
    {glm::vec2(0.5, 0), glm::vec2(0, 1), SDL_SCANCODE_I, SDL_SCANCODE_K},
    {glm::vec2(1, 0), glm::vec2(1, 0), SDL_SCANCODE_L, SDL_SCANCODE_J}
  };

private:
  void onHit(Projectile* with, const glm::vec2& collision_point, glm::vec2* normal);
};

class SliderLocalPlayer : public Slider {
public:
  SliderLocalPlayer() = delete;
  SliderLocalPlayer(const glm::vec2& pos, float rot) : Slider(GameObjectType::LocalPlayer, pos, rot) {
  };

  void update(const Uint8* keys, uint32_t elapsed_us) override;
};

class SliderComputerEnemy : public Slider {
public:
  SliderComputerEnemy() = delete;
  SliderComputerEnemy(const glm::vec2& pos, float rot) : Slider(GameObjectType::ComputerEnemy, pos, rot) {
    keys_state_.resize(7);
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override;

private:
  std::vector<bool> keys_state_;
};