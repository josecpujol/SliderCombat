#include "PowerUps.h"

PowerUp::PowerUp(GameObjectType type, const glm::vec2& pos) : GameObject(type, glm::vec3(pos, 0.0f), 0.0f, 0.0f) {
  // Set the models based on type
  base_model_.setObject3d(ResourcesManager::getInstance().getModel3d(ModelType::kPowerUps)->getObject3d("base"));
  base_model_.setOpenGlProgram(ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d));
  std::string powerup_model;
  if (type == GameObjectType::HealthPowerup) {
    powerup_model = "health";
  }
  assert(!powerup_model.empty());
  powerup_model_.setObject3d(ResourcesManager::getInstance().getModel3d(ModelType::kPowerUps)->getObject3d(powerup_model));
  powerup_model_.setOpenGlProgram(ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d));
  base_model_.setTranslation(glm::vec3(pos, 0.1f));
  powerup_model_.setTranslation(glm::vec3(pos, initial_height_));
  powerup_model_.setScale(glm::vec3(0.7f));
  powerup_model_.setScale(glm::vec3(0.5f));

  enable();
}

void PowerUp::onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) {
  if (with->getType() == GameObjectType::LocalPlayer || with->getType() == GameObjectType::ComputerEnemy) {
    disableFor(1min);
  }
}

void PowerUp::enable() {
  setCollisionArea(CollisionArea(Circle(glm::vec2(0.f), 0.7f)));
  is_visible_ = true;
}

void PowerUp::disableFor(Duration duration) {
  unsetCollisionArea();
  is_visible_ = false;
  enable_cooldown_ = duration;
}

void PowerUp::update(uint32_t elapsed_us) {
  // Check to reenable again
  if (enable_cooldown_ >= 0s) {
    enable_cooldown_ -= std::chrono::microseconds(elapsed_us);
  }

  if (!is_visible_ && enable_cooldown_ < 0s) {
    enable();
  }

  rotation_ += (elapsed_us * 120) / 1000000.f;
  powerup_model_.setRotationZ(rotation_);
  float amplitude = 0.5f;
  time_elapsed_us_ += elapsed_us;
  float height = initial_height_ + (amplitude / 2) * (1 - cos((time_elapsed_us_) / 1000000.f));
  glm::vec3 pos(getPosition());
  pos.z = height;
  powerup_model_.setTranslation(pos);
}

void PowerUp::render(Scene& scene) {
  if (is_visible_) {
    scene.render(base_model_);
    scene.render(powerup_model_);
  }
}
