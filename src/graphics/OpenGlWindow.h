#pragma once

#include "SDL.h"
#include "OpenGlResources.h"

class OpenGlWindow {
public:
  OpenGlWindow() {};
  ~OpenGlWindow();
  bool create(int width, int heigh);
  void display();

private:
  SDL_Window* window_ = nullptr;
  SDL_GLContext gl_context_;
  OpenGlResources opengl_;
};