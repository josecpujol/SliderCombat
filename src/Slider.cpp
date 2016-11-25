#include "Slider.h"

#include <GL/glew.h>

#include "Logger.h"
#include "EventManager.h"

void SliderLocalPlayer::update(const Uint8* keys, uint32_t elapsed_us) {
  // Positions wrt local system reference 
  glm::vec2 vel_rel(0, 0);
  float vel_rot = 0;

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
      Event event(EventType::Fire, this);
      event.send();
    }
  }

  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);
  rot_z_ += (vel_rot * elapsed_us) / 1000000;

  float rot_z_rad = glm::radians(rot_z_);

  // convert local positions to global
  pos_.x += (pos_rel.x * cos(rot_z_rad) + pos_rel.y * (-sin(rot_z_rad)));
  pos_.y += (pos_rel.x * sin(rot_z_rad) + pos_rel.y * cos(rot_z_rad));
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
      keys_state_[i] = !keys_state_[i];
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
      Event event(EventType::Fire, this);
      event.send();
    }
  }

  // update relative position
  glm::vec2 pos_rel = vel_rel * ((float)elapsed_us / 1000000);
  rot_z_ += (vel_rot * elapsed_us) / 1000000;

  float rot_z_rad = glm::radians(rot_z_);

  // convert local positions to global
  pos_.x += (pos_rel.x * cos(rot_z_rad) + pos_rel.y * (-sin(rot_z_rad)));
  pos_.y += (pos_rel.x * sin(rot_z_rad) + pos_rel.y * cos(rot_z_rad));
}

void Slider::onCollision(GameObject* with) {

}

void Slider::render() {
  glTranslatef(pos_.x, pos_.y, pos_.z);

  glRotated((float)rot_z_, 0, 0, 1);

  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0, 1, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0, 0, 1);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();
}
