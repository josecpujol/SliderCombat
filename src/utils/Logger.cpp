#include "Logger.h"


LoggerOpenGl::LoggerOpenGl() {
  bm_font_ = ResourcesManager::getInstance().getBitmapFont(FontType::kPrototype);
  fixed_lines_.resize(num_fixed_lines);
}

void LoggerOpenGl::log(const std::string &str) {
  lines_.push_back(str);
  if (lines_.size() > max_lines_) {
    lines_.pop_front();
  }
}

void LoggerOpenGl::log(int line, const std::string &str) {
  assert(line >= 0 && line < num_fixed_lines);
  fixed_lines_[line] = str;
}

void LoggerOpenGl::releaseResources() {
  bm_font_ = nullptr;
}

void LoggerOpenGl::render() {
  int width, height;
  ResourcesManager::getInstance().getWindowDimensions(&width, &height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(1.f, -1.f, 1.f);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_COLOR_MATERIAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  int line_number = 0;
  int font_size = 32;
  int line_height = font_size + 2;
  // regular lines
  for (auto& line : lines_) {
    int x = 0;
    int y = (max_lines_ - line_number++) * line_height;
    bm_font_->render(line, x, y, font_size);
  }

  // fixed lines
  line_number = 0;
  for (auto& line : fixed_lines_) {
    int x = 0;
    int y = line_number++ * line_height;
    bm_font_->render(line, x, y, font_size);
  }
  
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
}
