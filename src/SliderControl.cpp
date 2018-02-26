#include "SliderControl.h"

#include <utility>
#include <iomanip>

#include "SDL.h"
#include "ResourcesManager.h"
#include "Slider.h"
#include "Logger.h"

SliderCommands& SliderCommands::operator+=(const SliderCommands& rhs) {
  this->fire = this->fire || rhs.fire;

  this->left_thruster_foward = this->left_thruster_foward || rhs.left_thruster_foward;
  this->left_thruster_backwards = this->left_thruster_backwards || rhs.left_thruster_backwards;
  this->left_thruster_left = this->left_thruster_left || rhs.left_thruster_left;
  this->left_thruster_right = this->left_thruster_right || rhs.left_thruster_right;

  this->right_thruster_foward = this->right_thruster_foward || rhs.right_thruster_foward;
  this->right_thruster_backwards = this->right_thruster_backwards || rhs.right_thruster_backwards;
  this->right_thruster_left = this->right_thruster_left || rhs.right_thruster_left;
  this->right_thruster_right = this->right_thruster_right || rhs.right_thruster_right;

  return *this;
}

SliderCommands ManualSliderControl::update(uint32_t elapsed_us) {
  SliderCommands commands;
  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  if (keys[SDL_SCANCODE_W]) {
    commands.left_thruster_foward = true;
  }
  if (keys[SDL_SCANCODE_S]) {
    commands.left_thruster_backwards = true;
  }
  if (keys[SDL_SCANCODE_A]) {
    commands.left_thruster_left = true;
  }
  if (keys[SDL_SCANCODE_D]) {
    commands.left_thruster_right = true;
  }

  if (keys[SDL_SCANCODE_I]) {
    commands.right_thruster_foward = true;
  }
  if (keys[SDL_SCANCODE_K]) {
    commands.right_thruster_backwards = true;
  }
  if (keys[SDL_SCANCODE_J]) {
    commands.right_thruster_left = true;
  }
  if (keys[SDL_SCANCODE_L]) {
    commands.right_thruster_right = true;
  }

  if (keys[SDL_SCANCODE_SPACE]) {
    commands.fire = true;
  }

  // Joystick
  const JoystickState* joystick_state = ResourcesManager::getInstance().getJoystickState();
  if (joystick_state && joystick_state->buttons.size() > 6) {
    if (joystick_state->buttons[0]) commands.right_thruster_right = true;
    if (joystick_state->buttons[1]) commands.right_thruster_backwards = true;
    if (joystick_state->buttons[2]) commands.right_thruster_foward = true;
    if (joystick_state->buttons[3]) commands.right_thruster_left = true;

    if (joystick_state->buttons[4] || joystick_state->buttons[5]) commands.fire = true;
    
    if (joystick_state->up) commands.left_thruster_foward = true;
    if (joystick_state->down) commands.left_thruster_backwards = true;
    if (joystick_state->right) commands.left_thruster_right = true;
    if (joystick_state->left) commands.left_thruster_left = true;

  }
  
  return std::move(commands);
}

SliderCommands AiSliderControl::rotateTo(const glm::vec2& point_to) {
  SliderCommands commands;

  glm::vec2 point_to_vector = point_to - glm::vec2(slider_->getPosition());
  point_to_vector = glm::normalize(point_to_vector);
  float angle_deg = glm::degrees(glm::orientedAngle(glm::vec2(1.f, 0.f), point_to_vector));
  float current_rot = slider_->getRotation();

  float error_orientation = angle_deg - current_rot;
  if (error_orientation > 180) error_orientation -= 360;
  if (error_orientation < -180) error_orientation += 360;
  LOG_DEBUG(error_orientation);
  float epsilon = 1;
  if (error_orientation > epsilon) {
    commands.right_thruster_foward = true;
    commands.left_thruster_backwards = true;
  } else if (error_orientation < -epsilon ) {
    commands.right_thruster_backwards = true;
    commands.left_thruster_foward = true;
  }
  return std::move(commands);
}

// The idea is to activate all the thrusters whose composed final vector is closest to the one we want
SliderCommands AiSliderControl::moveTo(const glm::vec2& to) {
  SliderCommands commands;

  // Move to relative coordinates
  glm::vec2 to_in_slider_coords = to - glm::vec2(slider_->getPosition());
  float rot_deg = slider_->getRotation();

  to_in_slider_coords = applyRotation(to_in_slider_coords, -rot_deg);

 // LOG_DEBUG(to_in_slider_coords.x << " " << to_in_slider_coords.y);

  if (to_in_slider_coords.x > 0) {
    commands.right_thruster_foward = true;
    commands.left_thruster_foward = true;
  } else {
    commands.right_thruster_backwards = true;
    commands.left_thruster_backwards = true;
  }

  bool extra_side_power = abs(to_in_slider_coords.y) > abs(to_in_slider_coords.x);

  if (to_in_slider_coords.y > 0) {
    commands.right_thruster_left = true;
    commands.left_thruster_left = extra_side_power;
  } else {
    commands.right_thruster_right = true;
    commands.left_thruster_right = extra_side_power;
  }
  return std::move(commands);
}

SliderCommands AiSliderControl::update(uint32_t elapsed_us) {
  SliderCommands commands;

  elapsed_time_ += std::chrono::microseconds(elapsed_us);
  glm::vec2 point_to{50, 50};

  if (elapsed_time_ < 10s) point_to = glm::vec2{0, 0};
  else if (elapsed_time_ < 20s) point_to = glm::vec2{50, 0};
  else if (elapsed_time_ < 30s) point_to = glm::vec2{50, 150};
  commands += rotateTo(point_to);
  commands += moveTo(point_to);
  //commands.fire = true;

  return std::move(commands);
}