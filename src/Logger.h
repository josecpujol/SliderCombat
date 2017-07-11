#pragma once

#include <iostream>

enum class LogLevel {
  kError,
  kInfo, 
  kDebug
};

#define LOG_INFO(exp)  std::cout << "INFO: " << exp << std::endl;
#define LOG_DEBUG(exp)  std::cout << "DEBUG: " << exp << std::endl;
#define LOG_ERROR(exp) std::cout << "ERROR: " << exp << std::endl;