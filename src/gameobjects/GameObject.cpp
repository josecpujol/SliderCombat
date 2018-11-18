#include "GameObject.h"

void GameObject::renderCollisionArea() {
  collision_area_.render();
}

void GameObject::setCollisionArea(const CollisionArea& ca) {
  can_collide_ = true;
  collision_area_ = ca;
  setPose(pos_, rot_z_);  // Update the new collision area with current pose 
}

void GameObject::unsetCollisionArea() {
  can_collide_ = false;
}

const CollisionArea& GameObject::getCollisionArea() const {
  assert(canCollide());
  return collision_area_;
}

void GameObject::setPose(glm::vec3 pos, float rot) {
  displacement_ = pos - pos_;
  pos_ = pos;
  rot_z_ = rot;
  collision_area_.setPosition(glm::vec2(pos));
  collision_area_.setRotation(rot);

  if (poses_.size() == max_poses_) {
    poses_.pop_front();
  }
  poses_.push_back(Pose(pos, rot));
}

void GameObject::setPose(glm::vec3 pos) {
  setPose(pos, getRotation());
}

void GameObject::undoPose() {
  if (poses_.size() < 2) {
    return;  // don't have enough history
  }
  poses_.pop_back();
  Pose& pose = poses_.back();
  setPose(pose.position, pose.rotation);
}
