#include "SliderControl.h"

#include <utility>

#include "SDL.h"

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
  return std::move(commands);
}

SliderCommands AiSliderControl::getCommands() {
  SliderCommands commands;
  commands.fire = true;
  commands.left_thruster_foward = true;
  return std::move(commands);
}