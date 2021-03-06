#include "Projectile.h"

#include "graphics/OpenGlResources.h"

#include "utils/Logger.h"
#include "EventManager.h"
#include "gameobjects/Explosion.h"

Projectile::Projectile(glm::vec3 pos, float rot) : GameObject(GameObjectType::Projectile, pos, rot, 1.f) {
  Circle c;
  c.radius = 0.15f;
  setCollisionArea(CollisionArea(c));
  //model_ = new FireModel3d();
  model_.setObject3d(ResourcesManager::getInstance().getModel3d(ModelType::kProjectiles)->getObject3d("2_projectile"));
  model_.setOpenGlProgram(ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kMesh3dLighting));
};

void Projectile::update(uint32_t elapsed_us) {
 // model_->update(elapsed_us);

  // Positions wrt local system reference 
  glm::vec2 vel_rel(vel_, 0.f);
  
  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);

  float rot_z_rad = glm::radians(getRotation());

  // convert local positions to global
  glm::vec3 pos = getPosition();
  pos.x += (pos_rel.x * cos(rot_z_rad) + pos_rel.y * (-sin(rot_z_rad)));
  pos.y += (pos_rel.x * sin(rot_z_rad) + pos_rel.y * cos(rot_z_rad));
  setPose(pos);
}

void Projectile::onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) {
  if (with == nullptr) {  // map
    Object3d* obj3d = ResourcesManager::getInstance().getModel3d(ModelType::kProjectiles)->getObject3d("2_particle");
    AddObjectEvent event1(new Explosion(getPosition(), 700ms, 4, obj3d));
    event1.send();
  } else if (with->getType() == GameObjectType::Projectile) {
    Object3d* obj3d = ResourcesManager::getInstance().getModel3d(ModelType::kProjectiles)->getObject3d("2_particle");
    AddObjectEvent event1(new Explosion(getPosition(), 300ms, 4, obj3d));
    event1.send();
  } else if (with->getType() == GameObjectType::HealthPowerup) {
    return;
  }
  
  RemoveObjectEvent event2(this);
  event2.send();
}

void Projectile::render(Scene& scene) {
  model_.setTranslation(getPosition());
  model_.setRotationZ(getRotation());
  model_.setScale(glm::vec3(0.15f));
  scene.render(model_);
}
