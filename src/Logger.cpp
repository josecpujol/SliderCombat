#include "Logger.h"


LoggerOpenGl::LoggerOpenGl() {
  font_ = ResourcesManager::getInstance().getFont(FontType::kRobotoCondensed);
  textures_lines_.resize(max_lines_);
  glGenTextures(textures_lines_.size(), textures_lines_.data());

  fixed_lines_.resize(num_fixed_lines);
  textures_fixed_lines_.resize(num_fixed_lines);
  glGenTextures(textures_fixed_lines_.size(), textures_fixed_lines_.data());

}

void LoggerOpenGl::log(const std::string &str) {
  GLuint texture = textures_lines_[next_available_texture_line_index_];
  next_available_texture_line_index_ = (next_available_texture_line_index_ + 1) % textures_lines_.size();
  lines_.emplace_back(font_, texture, str);
  if (lines_.size() > max_lines_) {
    lines_.pop_front();
  }
}

void LoggerOpenGl::log(int line, const std::string &str) {
  assert(line >= 0 && line < num_fixed_lines);
  GLuint texture = textures_fixed_lines_[line];
  fixed_lines_[line] = TextTexture(font_, texture, str);
}

void LoggerOpenGl::releaseResources() {
  font_ = nullptr;
  glDeleteTextures(textures_lines_.size(), textures_lines_.data());
  glDeleteTextures(textures_fixed_lines_.size(), textures_fixed_lines_.data());
}

void LoggerOpenGl::render() {
  int width, height;
  ResourcesManager::getInstance().getWindowDimensions(&width, &height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  int line_number = 0;
  int line_height = 20;
  // regular lines
  for (auto& line : lines_) {
    int x = 0;
    int y = (max_lines_ - line_number++) * line_height;
    line.draw(x, y);
  }

  // fixed lines
  line_number = 1;
  for (auto& line : fixed_lines_) {
    int x = 0;
    int y = height - line_number++ * line_height;
    line.draw(x, y);
  }
  
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

}