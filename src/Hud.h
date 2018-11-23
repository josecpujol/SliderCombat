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

  Level* level_ = nullptr;
  GLuint texture_health_;
  int screen_width_ = 0;
  int screen_height_ = 0;
};