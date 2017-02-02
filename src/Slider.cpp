#include "Slider.h"

#include <GL/glew.h>

#include "Logger.h"
#include "EventManager.h"
#include "Math.h"

void SliderLocalPlayer::update(const Uint8* keys, uint32_t elapsed_us) {
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, 0);
  float vel_rot = 0;

  bool fire_event = false;

  if (keys[SDL_SCANCODE_W]) vel_rel.y = vel_y_;
  if (keys[SDL_SCANCODE_S]) vel_rel.y = -vel_y_;
  if (keys[SDL_SCANCODE_A]) vel_rel.x = -vel_x_;
  if (keys[SDL_SCANCODE_D]) vel_rel.x = vel_x_;
  if (keys[SDL_SCANCODE_J]) vel_rot = vel_rot_;
  if (keys[SDL_SCANCODE_K]) vel_rot = -vel_rot_;
  if (keys[SDL_SCANCODE_SPACE]) {
    TimePoint current_time = Clock::now();
    if (last_shot_ + shot_cadence_ < current_time) {
      // shot event
      last_shot_ = current_time;
      fire_event = true;
    }
  }

  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);
  rot_z_ += (vel_rot * elapsed_us) / 1000000;

  // convert local positions to global
  pos_ += glm::vec3(applyRotation(pos_rel, rot_z_), 0.0);

  // Fire after we have updated the position and rotation
  if (fire_event) {
    float cannon_offset = 1.2f;  // TODO: change: now hardcoded to avoid collision with oneself
    float cannon_height = 0.7f;
    FireEvent event(
      this, 
      pos_ + glm::vec3(applyRotation(glm::vec2(0.0, cannon_offset), rot_z_), cannon_height),
      rot_z_);
    event.send();
  }
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

  i = 0;
  if (keys_state_[i++]) vel_rel.y = vel_y_;
  if (keys_state_[i++]) vel_rel.y = -vel_y_;
  if (keys_state_[i++]) vel_rel.x = -vel_x_;
  if (keys_state_[i++]) vel_rel.x = vel_x_;
  if (keys_state_[i++]) vel_rot = vel_rot_;
  if (keys_state_[i++]) vel_rot = -vel_rot_;
  if (keys_state_[i++]) {
    if (last_shot_ + shot_cadence_ < current_time) {
      // shot event
      last_shot_ = current_time;
      FireEvent event(this, pos_, rot_z_);
      event.send();
    }
  }

  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);
  rot_z_ += (vel_rot * elapsed_us) / 1000000;

  // convert local positions to global
  glm::vec2 inc_pos = applyRotation(pos_rel, rot_z_);
  pos_.x += inc_pos.x;
  pos_.y += inc_pos.y;
}

void Slider::onCollision(GameObject* with) {
  LOG_DEBUG("Slider: onCollision");
  health_ -= 5;
  LOG_DEBUG("Slider: health: " << health_);
}

void Slider::render() {
  glTranslatef(pos_.x, pos_.y, pos_.z);

  glRotated((float)rot_z_, 0, 0, 1);
  if (!model_) {  // lazy instantiation
    model_ = ResourcesManager::getInstance().getModel3d(ModelType::kTank);
  }
  model_->render();
/*  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0, 1, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0, 0, 1);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();*/
}
