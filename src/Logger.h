#pragma once

#include <iostream>
#include <sstream>
#include <list>

#include "ResourcesManager.h"
#include "OpenGlResources.h"

enum class LogLevel {
  kError,
  kInfo,
  kDebug
};

#define LOG_INFO(exp)  std::cout << "INFO: " << exp << std::endl;
#define LOG_DEBUG(exp)  std::cout << "DEBUG: " << exp << std::endl;
#define LOG_ERROR(exp) std::cout << "ERROR: " << exp << std::endl;

#define LOG_DEBUG_SCREEN(exp) { \
  std::ostringstream ss; \
  ss << "DEBUG: " << exp; \
  LoggerOpenGl::getInstance().log(ss.str()); \
}

#define LOG_ERROR_SCREEN(exp) { \
  std::ostringstream ss; \
  ss << "ERROR: " << exp; \
  LoggerOpenGl::getInstance().log(ss.str()); \
}

#define LOG_SCREEN_N(slot, exp) { \
  std::ostringstream ss; \
  ss << exp; \
  LoggerOpenGl::getInstance().log(slot, ss.str()); \
}

class LoggerOpenGl {
public:
  LoggerOpenGl(LoggerOpenGl const&) = delete;
  void operator=(LoggerOpenGl const&) = delete;

  static LoggerOpenGl& getInstance() {
    static LoggerOpenGl instance;
    return instance;
  }

  void releaseResources();

  void log(const std::string&);
  void log(int line, const std::string&);
  void render();

private:
  class TextTexture {
  public:
    TextTexture() {};
    TextTexture(TTF_Font* font, GLint texture, const std::string& text) : texture_(texture), init_(true) {
      glBindTexture(GL_TEXTURE_2D, texture);

      SDL_Color color = {255, 150, 0, 0};
      SDL_Surface * surface = TTF_RenderText_Blended(font, text.c_str(), color);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

      w_ = surface->w;
      h_ = surface->h;
      SDL_FreeSurface(surface);
    }

    void draw(int x, int y) {
      if (!init_) return;
      glBindTexture(GL_TEXTURE_2D, texture_);
      glColor3f(1.f, 1.f, 1.f);  // The color is affected by glColor, unless glTexEnv is set with magic params
      glBegin(GL_QUADS);
      glTexCoord2f(0.f, 1.f); glVertex2f((float)x, (float)y);
      glTexCoord2f(1.f, 1.f); glVertex2f((float)(x + w_), (float)y);
      glTexCoord2f(1.f, 0.f); glVertex2f((float)(x + w_), (float)(y + h_));
      glTexCoord2f(0.f, 0.f); glVertex2f((float)x, (float)(y + h_));
      glEnd();
    }
  private:
    GLuint texture_;
    int w_;
    int h_;
    bool init_ = false;
  };
  LoggerOpenGl();
  int max_lines_ = 10;
  int num_fixed_lines = 5;
  std::list<TextTexture> lines_;
  std::vector<TextTexture> fixed_lines_;
  TTF_Font* font_ = nullptr;
  std::vector<GLuint> textures_lines_;
  std::vector<GLuint> textures_fixed_lines_;
  int next_available_texture_line_index_ = 0;
};
