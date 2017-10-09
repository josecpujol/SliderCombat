#pragma once

#include <memory>
#include <vector>

#include "SDL.h"
#include "OpenGlWindow.h"
#include "Level.h"

class TheGame {
public:
  TheGame();
  ~TheGame();
  bool init();
  void runLoop();
  
private:
  void oneIteration();
  void processEvents(const std::vector<SDL_Event>& events);
  std::unique_ptr<OpenGlWindow> window_;  // Need to destroy this before this class' destructor
  std::unique_ptr<Level> stage_;
  SDL_Joystick *joystick_ = nullptr;
  uint32_t target_fps_ = 60;

  // main loop properties: we need to decouple from a single iteration function
  int counter_ = 0;
  bool done_ = false;  // main loop exit condition
};

