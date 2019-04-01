#include "Hud.h"

#include "Level.h"
#include "graphics/OpenGlState.h"

Hud::Hud(Level* level) : level_(level) {
}

Hud::~Hud() {
}

void Hud::drawHealthBar() {
  ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d)->use();
  
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
  std::vector<glm::vec2> coords;
  std::vector<glm::vec3> colors;


  coords.push_back(glm::vec2(distance_from_left, screen_height - distance_from_bottom));
  coords.push_back(glm::vec2(distance_from_left + length, screen_height - distance_from_bottom));
  coords.push_back(glm::vec2(distance_from_left + length, screen_height - distance_from_bottom - bar_height));
  coords.push_back(glm::vec2(distance_from_left, screen_height - distance_from_bottom - bar_height));
  

  colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
  colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

  OpenGlResources::drawPolygon(coords, colors, GL_TRIANGLE_FAN);
  
  // Draw heath bar on top
  int lenght_health = (health.getValue() * length) / health.getMaxValue();
  int reduction = 5;
   
  coords.clear();
  colors.clear();

  coords.push_back(glm::vec2(distance_from_left, screen_height - distance_from_bottom - reduction));
  coords.push_back(glm::vec2(distance_from_left + lenght_health, screen_height - distance_from_bottom - reduction));
  coords.push_back(glm::vec2(distance_from_left + lenght_health, screen_height - distance_from_bottom - bar_height + reduction));
  coords.push_back(glm::vec2(distance_from_left, screen_height - distance_from_bottom - bar_height + reduction));
  
  colors.push_back(glm::vec3(0.6f, 0.1f, 0.02f));
  colors.push_back(glm::vec3(0.6f, 0.1f, 0.02f));
  colors.push_back(glm::vec3(0.6f, 0.1f, 0.02f));
  colors.push_back(glm::vec3(0.6f, 0.1f, 0.02f));

  OpenGlResources::drawPolygon(coords, colors, GL_TRIANGLE_FAN);
}

void Hud::display() {
  // Set opengl state to hud mode
  ResourcesManager::getInstance().getWindowDimensions(&screen_width_, &screen_height_);
  
  glDisable(GL_DEPTH_TEST);

  OpenGlState::getInstance().matrixMode(MatrixMode::kProjection);
  glm::mat4 perspective = glm::ortho(0.0f, float(screen_width_), float(screen_height_), 0.f, -1.f, 1.f);
  OpenGlState::getInstance().loadMatrix(perspective);

  OpenGlState::getInstance().matrixMode(MatrixMode::kModelView);
  OpenGlState::getInstance().loadIdentity();
  drawHealthBar();
  
  glEnable(GL_DEPTH_TEST);
}