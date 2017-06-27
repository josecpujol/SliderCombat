#include "Fire.h"

#include "OpenGlResources.h"

#include "Logger.h"
#include "EventManager.h"

Fire::Fire(glm::vec3 pos, float rot) : GameObject(GameObjectType::Fire, pos, rot, 1.f) {
  Circle c;
  c.radius = 0.2f;
  setCollisionArea(CollisionArea(c));
};

void Fire::update(const Uint8* keys, uint32_t elapsed_us) {
  update_counter_++;
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

void Fire::onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) {
  RemoveObjectEvent event(this);
  event.send();
}

void Fire::render() {
  glm::vec3 pos = getPosition();
  float rot_z = getRotation();
  glTranslatef(pos.x, pos.y, pos.z);

  glRotated((float)rot_z, 0, 0, 1);

  glColor3f(1, 1, 1);
  glRotatef(-(float)(update_counter_) * 10, 1, 0, 0);
  OpenGlResources::drawCircle(0.2f, 10);
}
