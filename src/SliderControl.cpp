#include "SliderControl.h"

#include <utility>

#include "SDL.h"
#include "ResourcesManager.h"

SliderCommands ManualSliderControl::getCommands() {
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

SliderCommands AiSliderControl::getCommands() {
  SliderCommands commands;
  commands.fire = true;
  commands.left_thruster_foward = true;
  return std::move(commands);
}