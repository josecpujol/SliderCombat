#include "Fire.h"

#include <GL/glew.h>

#include "Logger.h"
#include "EventManager.h"

void Fire::update(const Uint8* keys, uint32_t elapsed_us) {
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, vel_y_);
  
  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);

  float rot_z_rad = glm::radians(rot_z_);

  // convert local positions to global
  pos_.x += (pos_rel.x * cos(rot_z_rad) + pos_rel.y * (-sin(rot_z_rad)));
  pos_.y += (pos_rel.x * sin(rot_z_rad) + pos_rel.y * cos(rot_z_rad));
}

void Fire::onCollision(GameObject* with) {
  Event event(EventType::RemoveObject, this);
  event.send();
}

void Fire::render() {
  glTranslatef(pos_.x, pos_.y, pos_.z);

  glRotated((float)rot_z_, 0, 0, 1);

  glColor3f(1, 1, 1);
  glBegin(GL_TRIANGLES);
  glVertex3f(0.5f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-0.5f, 0.0f, 0.0f);
  glEnd();
}
