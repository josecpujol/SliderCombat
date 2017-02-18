#pragma once

#include "SDL.h"

#include "GameObject.h"

class Fire : public GameObject {
public:
  Fire(glm::vec3 pos, float rot) : GameObject(GameObjectType::Fire, pos, rot) {
    Circle c;
    c.radius = 0.2f;
    setCollisionArea(CollisionArea(c));
  };
  void update(const Uint8* keys, uint32_t elapsed_us) override;
  void render() override;
  void onCollision(GameObject* with) override;


private:
  // m/s
  float vel_ = 20.0;
};