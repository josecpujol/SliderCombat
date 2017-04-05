#include "Slider.h"

#include <GL/glew.h>

#include "Logger.h"
#include "EventManager.h"
#include "Math.h"

void SliderLocalPlayer::update(const Uint8* keys, uint32_t elapsed_us) {
  float elapsed_secs = (float)elapsed_us / 1000000;
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, 0);
  float vel_rot = 0;

  bool fire_event = false;

  for (int i = 0; i < 4; i++) {
    propellers_[i].update(keys);
  }
  
  if (keys[SDL_SCANCODE_SPACE]) {
    TimePoint current_time = Clock::now();
    if (last_shot_ + shot_cadence_ < current_time) {
      // shot event
      last_shot_ = current_time;
      fire_event = true;
    }
  }

  float torque = 0;
  glm::vec2 force(0);
  for (int i = 0; i < 4; i++) {
    torque += propellers_[i].getTorque();
    force += propellers_[i].getForceVector();
  }
  
  applyForceAndTorque(torque, force, elapsed_secs);
  
  // Fire after we have updated the position and rotation
  if (fire_event) {
    float cannon_offset = 1.2f;  // TODO: change: now hardcoded to avoid collision with oneself
    float cannon_height = 0.7f;
    float rot_z = getRotation();
    glm::vec3 pos = getPosition();
    FireEvent event(
      this, 
      pos + glm::vec3(applyRotation(glm::vec2(0.0, cannon_offset), rot_z), cannon_height),
      rot_z);
    event.send();
  }
}



void Slider::applyForceAndTorque(float torque, glm::vec2 force, float elapsed_secs) {
  float drag_coeff_rotation = 2.f;
  float moment_inertial = mass_ * 0.05f;  // tweaking physics...

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

  glm::vec2 local_acceleration = (1.f / mass_) * force;
  // convert local positions to global
  global_speed_ += applyRotation(local_acceleration, rot_z) * elapsed_secs;
  glm::vec3 pos = getPosition();
  pos += glm::vec3(global_speed_, 0) * elapsed_secs;

  setPosition(pos);
  setRotation(new_rot_z);
}

void SliderComputerEnemy::update(const Uint8* keys, uint32_t elapsed_us) {
  TimePoint current_time = Clock::now();
  uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch()).count();
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, 0);
  float vel_rot = 0;
  int i;
  for (i = 0; i < 7; i++) {
    if (ms % (100 + i) == 0) {
     //   keys_state_[i] = !keys_state_[i];
    }
  }

  float elapsed_secs = (float)elapsed_us / 1000000;
  //applyForceAndTorque(0, elapsed_secs);
}

void Slider::onCollision(GameObject* with, const BoundVector2& collision_point) {
  LOG_DEBUG("Slider: onCollision");
  if (with->getType() == GameObjectType::Fire) {
    health_ -= 5;
    LOG_DEBUG("Slider: health: " << health_);
  }
}

void Slider::render() {
  glm::vec3 pos = getPosition();
  glTranslatef(pos.x, pos.y, pos.z);

  glRotatef(getRotation(), 0, 0, 1);
  if (!model_) {  // lazy instantiation
    model_ = ResourcesManager::getInstance().getModel3d(ModelType::kTank);
  }
  model_->render();
}
