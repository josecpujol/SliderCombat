#include "Projectile.h"

#include "OpenGlResources.h"

#include "Logger.h"
#include "EventManager.h"
#include "Explosion.h"

Projectile::Projectile(glm::vec3 pos, float rot) : GameObject(GameObjectType::Projectile, pos, rot, 1.f) {
  Circle c;
  c.radius = 0.15f;
  setCollisionArea(CollisionArea(c));
  //model_ = new FireModel3d();
  model_ = ResourcesManager::getInstance().getModel3d(ModelType::kProjectiles)->getObject3d("2");
};

void Projectile::update(const Uint8* keys, uint32_t elapsed_us) {
 // model_->update(elapsed_us);

  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, vel_);
  
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
  AddObjectEvent event1(new Explosion(getPosition(), 2s, 1));
  event1.send();

  RemoveObjectEvent event2(this);
  event2.send();
}

void Projectile::render() {
  glm::vec3 pos = getPosition();
  float rot_z = getRotation();

  glTranslatef(pos.x, pos.y, pos.z);

  glRotated((float)rot_z, 0, 0, 1);
  //glColor3f(1, 1, 1);
  //glRotatef(-(float)(update_counter_) * 10, 1, 0, 0);
  //OpenGlResources::drawCircle(0.2f, 10);
 // model_->render();
  glScalef(0.15f, 0.15f, 0.15f);
  model_->render();
}
