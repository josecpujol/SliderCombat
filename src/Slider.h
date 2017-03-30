#pragma once

#include "SDL.h"

#include <vector>

#include "GameObject.h"
#include "Time.h"
#include "Math.h"
#include "ResourcesManager.h"

class Propeller {
public:
  Propeller(glm::vec2 location, glm::vec2 direction, Uint8 direct_key, Uint8 reverse_key) :
    location_(location), direction_(direction), direct_key_(direct_key), reverse_key_(reverse_key) {
    glm::vec3 v1 = glm::vec3(location_, 0);
    glm::vec3 v2 = glm::vec3(direction_, 0);

    unit_torque_ = (glm::cross(v1, v2)).z;
  }

  void update(const Uint8* keys) {
    if (keys[direct_key_]) direct();
    else if (keys[reverse_key_]) reverse();
    else neutral();
  }

  glm::vec2 getForceVector() const {
    return force_ * direction_;
  }

  // Assuming CoM to be in 0,0
  float getTorque() const {
    return force_ * unit_torque_;
  }

private:
  void direct() { force_ = force_max_; }
  void reverse() { force_ = -force_max_; }
  void neutral() { force_ = 0.f; }
  float unit_torque_ = 0.f;  // precompute cross vector and scale with force
  float force_ = 0.0f;
  float force_max_ = 100.f;
  glm::vec2 location_;
  glm::vec2 direction_;
  Uint8 direct_key_;
  Uint8 reverse_key_;
};

class Slider : public GameObject {
public:
  Slider() = delete;
  Slider(GameObjectType type, const glm::vec2& pos, float rot) : 
    GameObject(GameObjectType::LocalPlayer, glm::vec3(pos, 0.1), rot) {
    Rectangle rect(pos, glm::vec2(2, 2), rot);
    setCollisionArea(CollisionArea(rect));
    
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override {};
  void render() override;
  void onCollision(GameObject* with, const glm::vec2& collision_point) override;


protected:

  void applyForceAndTorque(const float torque, glm::vec2 force, float elapsed_secs);
 
  float mass_ = 10.f;
  glm::vec2 global_speed_;  // global coordinates
  float angular_speed_ = 0.f;
  float vel_rot_ = 20.0;
  Duration shot_cadence_ = 200ms;
  const int max_health = 100;
  int health_ = max_health;
  TimePoint last_shot_;
  Model3d* model_ = nullptr;

  struct ImpactInfo {
    glm::vec2 location;
    glm::vec2 force;
  };
  std::vector<ImpactInfo> impacts_;  // local coordinates

  Propeller propellers_[4] = {
    {glm::vec2(-0.5, 0), glm::vec2(0, 1), SDL_SCANCODE_W, SDL_SCANCODE_S},
    {glm::vec2(-1, 0), glm::vec2(-1, 0), SDL_SCANCODE_A, SDL_SCANCODE_D},
    {glm::vec2(0.5, 0), glm::vec2(0, 1), SDL_SCANCODE_I, SDL_SCANCODE_K},
    {glm::vec2(1, 0), glm::vec2(1, 0), SDL_SCANCODE_L, SDL_SCANCODE_J}
  };
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