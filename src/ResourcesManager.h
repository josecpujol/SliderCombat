#pragma once

#include <memory>
#include "Model3d.h"

enum class ModelType {
  kCube,
  kVio,
  kTank
};

class ResourcesManager {
public:
  ResourcesManager(ResourcesManager const&) = delete;
  void operator=(ResourcesManager const&) = delete;
  
  static ResourcesManager& getInstance() {
    static ResourcesManager instance;
    return instance;
  }

  void setWindowDimensions(int w, int h);
  void getWindowDimensions(int* w, int* h);
  bool loadModels();
  Model3d* getModel3d(ModelType);
private:
  ResourcesManager() {}

  int window_width_ = 0;
  int window_height_ = 0;
  std::map<ModelType, std::unique_ptr<Model3d>> models_;
};