#pragma once

#include <memory>
#include "Model3d.h"
#include "Map.h"


enum class ModelType {
  kTiles,
  kTank
};

class ResourcesManager {
public:
  ResourcesManager(ResourcesManager const&) = delete;
  void operator=(ResourcesManager const&) = delete;

  static std::string getResourceBaseDirectory();
  
  static ResourcesManager& getInstance() {
    static ResourcesManager instance;
    return instance;
  }

  void setWindowDimensions(int w, int h);
  void getWindowDimensions(int* w, int* h);
  bool loadResources();

  Model3d* getModel3d(ModelType);
  Map* getMap();
private:
  ResourcesManager() {}

  bool loadModels();
  bool loadMaps();

  int window_width_ = 0;
  int window_height_ = 0;
  std::map<ModelType, std::unique_ptr<Model3d>> models_;
  std::unique_ptr<Map> map_;
};