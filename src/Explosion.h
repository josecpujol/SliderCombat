#pragma once

#include "GameObject.h"
#include "Time.h"
#include "Logger.h"

struct AnimatedPose {
  glm::vec3 pos_init;
  glm::vec3 pos_end;
  glm::vec3 rotation_init;
  glm::vec3 rotation_end;
  float scale_init;
  float scale_end;

  // current params
  glm::vec3 pos;
  glm::vec3 rotation;
  float scale;

  // t: 0->1
  // will update current params
  void update(float t) {
    pos = pos_init * (1 - t) + pos_end * t;
    rotation = rotation_init * (1 - t) + rotation_end * t;
    scale = scale_init * (1 - t) + scale_end * t;
  }
};

class Explosion : public GameObject {
public:
  Explosion(const glm::vec3& pos, Duration duration, float speed, Object3d* particle);
  ~Explosion() = default;

  void render() override;
  void update(uint32_t elapsed_us) override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override { assert(false); };

private:
  std::chrono::microseconds total_duration_;
  std::chrono::microseconds duration_{0};
  std::vector<AnimatedPose> particles_poses_;
  Object3dHolder particle_;
};