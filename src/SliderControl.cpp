#include "SliderControl.h"

#include <utility>

#include "SDL.h"
#include "ResourcesManager.h"
#include "Slider.h"
#include "Logger.h"

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

SliderCommands AiSliderControl::update(uint32_t elapsed_us) {
  SliderCommands commands;

  elapsed_time_ += std::chrono::microseconds(elapsed_us);
  glm::vec2 point_to(10.f);
  if (std::chrono::duration_cast<std::chrono::seconds>(elapsed_time_).count() % 5) {
  //  point_to = glm::vec2(25.f, 0.f);
  }
  
  // Target rotation
  glm::vec2 point_to_vector = point_to - glm::vec2(slider_->getPosition());
  point_to_vector = glm::normalize(point_to_vector);
  float angle_rad = glm::orientedAngle(glm::vec2(1.f, 0.f), point_to_vector);
  float angle_deg = (angle_rad * 180.f) / glm::pi<float>();
  float current_rot = slider_->getRotation();

  float error_orientation = angle_deg - current_rot;
  LOG_DEBUG("angle: " << angle_deg << ", current rot: " << current_rot << ", error: " << error_orientation);
  if (error_orientation > 0) {
    commands.right_thruster_foward = true;
    commands.left_thruster_backwards = true;
  } else {
    commands.right_thruster_backwards = true;
    commands.left_thruster_foward = true;
  }



  //commands.fire = true;
  //commands.left_thruster_foward = true;
  return std::move(commands);
}