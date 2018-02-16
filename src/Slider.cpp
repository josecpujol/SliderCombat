#include "Slider.h"

#include "Logger.h"
#include "EventManager.h"
#include "Math.h"
#include "Projectile.h"


void Slider::shoot() {
  float cannon_offset = 1.35f;  // TODO: change: now hardcoded to avoid collision with oneself
  float cannon_height = 0.7f;
  float rot_z = getRotation();
  glm::vec3 pos = getPosition();
  AddObjectEvent event(
    new Projectile(pos + glm::vec3(applyRotation(glm::vec2(cannon_offset, 0.f), rot_z), cannon_height), rot_z)
    );

  event.send();

  shot_current_cooldown_ = shot_cooldown_duration_;
}

void Slider::applyForceAndTorque(float torque, glm::vec2 force, float elapsed_secs) {
  float drag_coeff_rotation = 2.f;
  float moment_inertial = getMass() * 0.05f;  // tweaking physics...

  // Compute forces: rotation
  torque -= drag_coeff_rotation * angular_speed_;  // apply friction
  float angular_acceleration = torque / moment_inertial;
  angular_speed_ += angular_acceleration * elapsed_secs;
  float rot_z = getRotation();
  float new_rot_z = rot_z + angular_speed_ * elapsed_secs;

  // max speed: max force / drag_coeff_linear_motion
  // drag_coeff_linear_motion for reaching 0.95 * max speed in t = -m * ln(1-0.95)/t

  // Compute forces: linear: sum vectors, it doesn't matter the origin
  float drag_coeff_linear_motion = 40.0f;
  force -= applyRotation(drag_coeff_linear_motion * global_speed_, -rot_z);  // apply friction

  glm::vec2 local_acceleration = (1.f / getMass()) * force;
  // convert local positions to global
  global_speed_ += applyRotation(local_acceleration, rot_z) * elapsed_secs;
  glm::vec3 pos = getPosition();
  pos += glm::vec3(global_speed_, 0) * elapsed_secs;

  setPose(pos, new_rot_z);
}

void Slider::onCollision(GameObject* with, const glm::vec2& collision_point, glm::vec2* normal) {
  if (with == nullptr) {  // Collision with map border
    assert(normal);
    // reflection vector: r = d - 2 dot(d.n)* n
    global_speed_ = global_speed_ - 2 * glm::dot(global_speed_, *normal) * (*normal);
    undoPose();
    return;
  }
  if (with->getType() == GameObjectType::Projectile) {
    Projectile* projectile = static_cast<Projectile*>(with);
    onHit(projectile, collision_point, normal);
  }
  if (with->getType() == GameObjectType::HealthPowerup) {
    health_.addValue(25);
  }
}

void Slider::onHit(Projectile* with, const glm::vec2& collision_point, glm::vec2* normal) {
  health_.addValue(-1 * with->getDamage());

  time_hit_duration_ = 0s;
  is_hit_ = true;

  float rot_z = getRotation();
  BoundVector2 collision_force;
  collision_force.origin = applyRotation(collision_point - glm::vec2(getPosition()), -rot_z);  // local coordinates
 
  // Impulse: F * t = m * Av
  // t = 0.01 seconds (arbitrary)
  // Av = displacement / 0.02
  // TODO: get Av directly from object instead of Ax/At
  collision_force.direction = applyRotation(glm::vec2(with->getDisplacement()), -rot_z);  // convert to local coordinates
  collision_force.direction *= (with->getMass() / 0.02) / 0.01;

  impacts_.push_back(Force(collision_force));
}

bool Slider::canShoot() const { 
  return shot_current_cooldown_ == 0s;
}

void Slider::updateHitState(uint32_t elapsed_us) {
  if (is_hit_) {
    std::chrono::milliseconds hit_max_duration = 200ms;
    int period_ms = 100;
    hit_duration_ += std::chrono::microseconds(elapsed_us);
    if (hit_duration_ > hit_max_duration) {
      is_hit_ = false;
      hit_duration_ = 0s;
    }
  }
}

void Slider::updateShotCooldown(uint32_t elapsed_us) {
  shot_current_cooldown_ -= std::chrono::microseconds(elapsed_us);
  if (shot_current_cooldown_ < 0s) {
    shot_current_cooldown_ = 0s;
  }
}

void Slider::update(uint32_t elapsed_us) {
  current_command_ = control_->update(elapsed_us);
  
  updateShotCooldown(elapsed_us);
  updateHitState(elapsed_us);
    
  float elapsed_secs = (float)elapsed_us / 1000000;
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, 0);

  bool fire_event = false;

  propellers_[0].update(current_command_.left_thruster_foward, current_command_.left_thruster_backwards);
  propellers_[1].update(current_command_.left_thruster_left, current_command_.left_thruster_right);
  propellers_[2].update(current_command_.right_thruster_foward, current_command_.right_thruster_backwards);
  propellers_[3].update(current_command_.right_thruster_right, current_command_.right_thruster_left);

  float torque = 0;
  glm::vec2 force(0);

  // Get force from propellers
  for (auto& propeller : propellers_) {
    torque += propeller.getTorque();
    force += propeller.getForceVector();
  }

  // Get force from impacts
  for (auto& impact : impacts_) {
    torque += impact.getTorque();
    force += impact.getForceFreeVector();
  }
  impacts_.clear();

  applyForceAndTorque(torque, force, elapsed_secs);

  // Fire after we have updated the position and rotation
  if (current_command_.fire && canShoot()) {
    shoot();
  }

};

void Slider::render() {
  // hit animation
#ifndef __EMSCRIPTEN__
  if (is_hit_) {
    int period_ms = 100;

    float factor = (cos(6.28f * hit_duration_.count() / period_ms) + 1) / 2;
    GLfloat color[] = {factor, factor, factor, 1.f};
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
  }
#endif

  glm::vec3 pos = getPosition();
  glTranslatef(pos.x, pos.y, pos.z);

  glRotatef(getRotation(), 0, 0, 1);
  if (!model_) {  // lazy instantiation
    model_ = ResourcesManager::getInstance().getModel3d(ModelType::kTank);
  }
  model_->render();

#ifndef __EMSCRIPTEN__
  if (is_hit_) {
    GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.f};
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
  }
#endif
}
