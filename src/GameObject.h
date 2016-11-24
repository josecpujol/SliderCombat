#pragma once

#include "SDL.h"
#include <glm/glm.hpp>

enum class GameObjectType {Undefined, Fire, LocalPlayer, ComputerEnemy};

class GameObject {
public:
  GameObject() = delete;
  GameObject(GameObjectType type, glm::vec3 pos, float rot) : type_(type), pos_(pos), rot_z_(rot) {}
  GameObject(const GameObject&) = delete;
  GameObject(const GameObject& obj, GameObjectType type) : type_(type) {
    this->pos_ = obj.pos_;
    this->rot_z_ = obj.rot_z_;
  }
  virtual void render() = 0;
  virtual void update(const Uint8* keys, uint32_t elapsed_us) = 0;
  virtual void onCollision(GameObject* with) = 0;
  GameObjectType getType() const { return type_;  }
  virtual ~GameObject() {}

  const glm::vec3& getPosition() { return pos_; }

protected:
  glm::vec3 pos_ = glm::vec3(0.0, 0.0, 0.1);
  float rot_z_ = 0.0;  // degrees

private:
  GameObjectType type_ = GameObjectType::Undefined;
};