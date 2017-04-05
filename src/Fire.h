#pragma once

#include "SDL.h"

#include "GameObject.h"

class Fire : public GameObject {
public:
  Fire(glm::vec3 pos, float rot);
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with, const BoundVector2& collision_point) override;


private:
  // m/s
  float vel_ = 20.0;
  int update_counter_ = 0;
};