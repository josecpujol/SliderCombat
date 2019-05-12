#pragma once

#include "SDL.h"

#include <vector>

#include "gameobjects/GameObject.h"
#include "gameobjects/SliderControl.h"
#include "utils/Time.h"
#include "mathnphysics/Math.h"
#include "mathnphysics/Physics.h"
#include "ResourcesManager.h"

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
  Propeller(glm::vec2 location, glm::vec2 direction) : 
    force_(BoundVector2(location, glm::normalize(direction))) {
  }

  void update(bool direct_command, bool reverse_command) {
    if (direct_command && reverse_command) neutral();
    else if (direct_command) direct();
    else if (reverse_command) reverse();
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
};

class Slider : public GameObject {
public:
  Slider() = delete;
  Slider(GameObjectType type, const Pose& pose) : 
    GameObject(type, glm::vec3(glm::vec2(pose.position), 0.1), pose.rotation, 10.f) {
 //   Rectangle rect(pos, glm::vec2(2, 2), rot);
 //   setCollisionArea(CollisionArea(rect)); 
    Circle circle(glm::vec2(pose.position), 1);
    setCollisionArea(CollisionArea(circle));
    model_.setObject3d(ResourcesManager::getInstance().getModel3d(ModelType::kTank)->getObject3d("tank"));
    model_.setOpenGlProgram(ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d));
  };
  void update(uint32_t elapsed_us) override;
  void render(Scene&) override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override;
  bool canShoot() const;

  Meter getHealth() const { return health_; }

protected:
  std::unique_ptr<SliderControl> control_;
  void applyForceAndTorque(const float torque, glm::vec2 force, float elapsed_secs);
  void shoot();

  glm::vec2 global_speed_;  // global coordinates
  float angular_speed_ = 0.f;
  const Duration shot_cooldown_duration_{200ms};
  Duration shot_current_cooldown_{0s};

  Duration hit_duration_{0s};

  Meter health_{100, 100};
  Object3dHolder model_;
  Duration time_hit_duration_;
  bool is_hit_{false};

  std::vector<Force> impacts_;  // local coordinates

  std::vector<Propeller> propellers_ = {
    {glm::vec2(0, 0.5), glm::vec2(1, 0)},
    {glm::vec2(0, 1), glm::vec2(0, 1)},
    {glm::vec2(0, -0.5), glm::vec2(1, 0)},
    {glm::vec2(0, -1), glm::vec2(0, -1)}
  };


private:
  void onHit(Projectile* with, const glm::vec2& collision_point, glm::vec2* normal);
  void updateHitState(uint32_t elapsed_us);
  void updateShotCooldown(uint32_t elapsed_us);
};

class SliderLocalPlayer : public Slider {
public:
  SliderLocalPlayer() = delete;
  SliderLocalPlayer(const Pose& pose) : Slider(GameObjectType::LocalPlayer, pose) {
    control_ = std::make_unique<ManualSliderControl>();
  };
};

class SliderComputerEnemy : public Slider {
public:
  SliderComputerEnemy() = delete;
  SliderComputerEnemy(const Pose& pose) : Slider(GameObjectType::ComputerEnemy, pose) {
    control_ = std::make_unique<AiSliderControl>(this);
  };
};