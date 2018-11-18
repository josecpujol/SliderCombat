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

  LoggerOpenGl();

  static LoggerOpenGl& getInstance() {
    static LoggerOpenGl instance;
    return instance;
  }

  void releaseResources();

  void log(const std::string&);
  void log(int line, const std::string&);
  void render();
  
  int max_lines_ = 10;
  int num_fixed_lines = 5;
  std::list<std::string> lines_;
  std::vector<std::string> fixed_lines_;
  BitmapFont* bm_font_ = nullptr;
};
