#include "GameObject.h"

void GameObject::renderCollisionArea() {
  collision_area_.render();
}

void GameObject::setCollisionArea(const CollisionArea& ca) {
  collision_area_ = ca;
  setPosition(pos_);
  setRotation(rot_z_);
}

void GameObject::setPosition(const glm::vec3& pos) {
  displacement_ = pos - pos_;
  pos_ = pos;
  collision_area_.setPosition(glm::vec2(pos));
}
void GameObject::setRotation(float rot) {
  rot_z_ = rot;
  collision_area_.setRotation(rot);
}