#include "Logger.h"


LoggerOpenGl::LoggerOpenGl() {
  font_ = ResourcesManager::getInstance().getFont(FontType::kRobotoCondensed, 20);
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
  glDisable(GL_COLOR_MATERIAL);
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

LoggerOpenGl::TextTexture::TextTexture(TTF_Font* font, GLint texture, const std::string& text) : texture_(texture), init_(true) {
  glBindTexture(GL_TEXTURE_2D, texture);

  SDL_Color color = {255, 255, 0, 255};
 // SDL_Color color_bg = {0, 255, 255, 255};
  SDL_Surface * surface1 = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Surface * surface2 = SDL_ConvertSurfaceFormat(surface1, SDL_PIXELFORMAT_RGBA8888, 0);
  SDL_FreeSurface(surface1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface2->w, surface2->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface2->pixels);

  w_ = surface2->w;
  h_ = surface2->h;
  SDL_FreeSurface(surface2);
}