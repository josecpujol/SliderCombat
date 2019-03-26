#pragma once

#include "ResourcesManager.h"
#include "graphics/OpenGlResources.h"

class Level;

class Hud {
public:
  Hud(Level* level);
  ~Hud();
  void display();
private:
  void drawHealthBar();

  OpenGlBuffer vertex_buffer_;
  OpenGlBuffer color_buffer_;
  Level* level_ = nullptr;
  int screen_width_ = 0;
  int screen_height_ = 0;
};