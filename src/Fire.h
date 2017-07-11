#pragma once

#include "SDL.h"

#include "GameObject.h"
#include "ResourcesManager.h"

class Fire : public GameObject {
public:
  Fire(glm::vec3 pos, float rot);
  ~Fire() {}
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) override;


private:
  // m/s
  float vel_ = 25.0;
  Object3d* model_ = nullptr;
};