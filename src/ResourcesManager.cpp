#include "ResourcesManager.h"

void ResourcesManager::setWindowDimensions(int w, int h) {
  window_width_ = w;
  window_height_ = h;
}

void ResourcesManager::getWindowDimensions(int* w, int* h) {
  *w = window_width_;
  *h = window_height_;
}