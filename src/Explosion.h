#pragma once

#include "GameObject.h"
#include "Time.h"
#include "Logger.h"

class Explosion : public GameObject {
public:
  Explosion(const glm::vec3& pos, Duration duration, float speed);
  ~Explosion() {
    LOG_DEBUG("Explosion destructor");
  }

  void render() override;
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override { assert(false); };

private:
  std::chrono::microseconds total_duration_;
  std::chrono::microseconds duration_{0};
  float speed_ = 0.0;  // units/sec
};