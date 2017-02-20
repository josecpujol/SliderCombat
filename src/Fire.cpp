#include "Fire.h"

#include "OpenGlResources.h"

#include "Logger.h"
#include "EventManager.h"

Fire::Fire(glm::vec3 pos, float rot) : GameObject(GameObjectType::Fire, pos, rot) {
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

  float rot_z_rad = glm::radians(rot_z_);

  // convert local positions to global
  glm::vec3 pos = getPosition();
  pos.x += (pos_rel.x * cos(rot_z_rad) + pos_rel.y * (-sin(rot_z_rad)));
  pos.y += (pos_rel.x * sin(rot_z_rad) + pos_rel.y * cos(rot_z_rad));
  setPosition(pos);
}

void Fire::onCollision(GameObject* with) {
  RemoveObjectEvent event(this);
  event.send();
}

void Fire::render() {
  glm::vec3 pos = getPosition();
  glTranslatef(pos.x, pos.y, pos.z);

  glRotated((float)rot_z_, 0, 0, 1);

  glColor3f(1, 1, 1);
  glRotatef(-(float)(update_counter_) * 5, 1, 0, 0);
  OpenGlResources::drawCircle(0.2, 10);
}
