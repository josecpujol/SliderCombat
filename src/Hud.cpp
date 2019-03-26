#include "Hud.h"

#include "Level.h"
#include "graphics/OpenGlState.h"

Hud::Hud(Level* level) : level_(level) {
}

Hud::~Hud() {
}

void Hud::drawHealthBar() {
  ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d)->use();
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  
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
  
  // Two bars
  float coords1[8] = {
    distance_from_left, screen_height - distance_from_bottom, 
    distance_from_left + length, screen_height - distance_from_bottom, 
    distance_from_left + length, screen_height - distance_from_bottom - bar_height,
    distance_from_left, screen_height - distance_from_bottom - bar_height,
  };

  float colors1[12] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
  };

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_.name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords1), coords1, GL_STATIC_DRAW);
  glVertexPointer(2, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_.name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors1), colors1, GL_STATIC_DRAW);
  glColorPointer(3, GL_FLOAT, 0, 0);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // Draw heath bar on top
  int lenght_health = (health.getValue() * length) / health.getMaxValue();
  int reduction = 5;

  float coords2[8] = {
    distance_from_left, screen_height - distance_from_bottom - reduction,
    distance_from_left + lenght_health, screen_height - distance_from_bottom - reduction,
    distance_from_left + lenght_health, screen_height - distance_from_bottom - bar_height + reduction,
    distance_from_left, screen_height - distance_from_bottom - bar_height + reduction
  };

  float colors2[12] = {
    0.6f, 0.1f, 0.02f,
    0.6f, 0.1f, 0.02f,
    0.6f, 0.1f, 0.02f,
    0.6f, 0.1f, 0.02f
  };

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_.name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords2), coords2, GL_STATIC_DRAW);
  glVertexPointer(2, GL_FLOAT, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_.name);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors2), colors2, GL_STATIC_DRAW);
  glColorPointer(3, GL_FLOAT, 0, 0);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void Hud::display() {
  // Set opengl state to hud mode
  ResourcesManager::getInstance().getWindowDimensions(&screen_width_, &screen_height_);
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_COLOR_MATERIAL);
  glDisable(GL_LIGHTING);

  OpenGlState::getInstance().matrixMode(MatrixMode::kProjection);
  glm::mat4 perspective = glm::ortho(0.0f, float(screen_width_), float(screen_height_), 0.f, -1.f, 1.f);
  OpenGlState::getInstance().loadMatrix(perspective);

  OpenGlState::getInstance().matrixMode(MatrixMode::kModelView);
  OpenGlState::getInstance().loadIdentity();
  drawHealthBar();
  
  glEnable(GL_DEPTH_TEST);
}