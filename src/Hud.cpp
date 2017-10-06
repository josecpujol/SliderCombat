#include "Hud.h"

#include "Level.h"

Hud::Hud(Level* level) : level_(level) {
  font_health_ = ResourcesManager::getInstance().getFont(FontType::kPrototype, 40);
  glGenTextures(1, &texture_health_);
}

Hud::~Hud() {
  glDeleteTextures(1, &texture_health_);
}

void Hud::drawHealthBar() {
  std::shared_ptr<const SliderLocalPlayer> local_player = level_->getLocalPlayer();
  Meter health = local_player->getHealth();
  int screen_width = 0;
  int screen_height = 0;
  ResourcesManager::getInstance().getWindowDimensions(&screen_width, &screen_height);

  // Draw background bar
  int distance_from_left = 30;
  int distance_from_bottom = 20;
  int length = 250;
  int bar_height = 40;
  glColor3f(0.f, 0.f, 0.0f);
  glBegin(GL_QUADS);
  glVertex2i(distance_from_left, screen_height - distance_from_bottom);
  glVertex2i(distance_from_left + length, screen_height - distance_from_bottom);
  glVertex2i(distance_from_left + length, screen_height - distance_from_bottom - bar_height);
  glVertex2i(distance_from_left, screen_height - distance_from_bottom - bar_height);
  glEnd();

  // Draw heath bar on top
  glColor3f(0.6f, 0.1f, 0.02f);
  int lenght_health = (health.getValue() * length) / health.getMaxValue();
  int reduction = 5;
  glBegin(GL_QUADS);
  glVertex2f(distance_from_left, screen_height - distance_from_bottom - reduction);
  glVertex2f(distance_from_left + lenght_health, screen_height - distance_from_bottom - reduction);
  glVertex2f(distance_from_left + lenght_health, screen_height - distance_from_bottom - bar_height + reduction);
  glVertex2f(distance_from_left, screen_height - distance_from_bottom - bar_height + reduction);
  glEnd();

}

void Hud::display() {
  // Set opengl state to hud mode
  ResourcesManager::getInstance().getWindowDimensions(&screen_width_, &screen_height_);
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, screen_width_, screen_height_, 0.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawHealthBar();
  
  glEnable(GL_DEPTH_TEST);
}