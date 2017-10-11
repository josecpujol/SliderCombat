#pragma once

#include <memory>
#include "Model3d.h"
#include "Map.h"
#include "SDL_ttf.h"
#include "OpenGlResources.h"


enum class ModelType {
  kTiles,
  kTank,
  kProjectiles,
  kPowerUps
};

enum class FontType {
  kRobotoCondensed,
  kPrototype
};

enum class OpenGlPrograms {
  kBasic
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

  void releaseResources();

  void setWindowDimensions(int w, int h);
  void getWindowDimensions(int* w, int* h);
  bool loadResources();

  Model3d* getModel3d(ModelType);
  TTF_Font* getFont(FontType, int point_size);
  Map* getMap();
  
private:
  ResourcesManager() {}

  bool loadModels();
  bool loadMaps();
  bool loadFonts();
  bool loadOpenGlPrograms();

  int window_width_ = 0;
  int window_height_ = 0;
  std::map<ModelType, std::unique_ptr<Model3d>> models_;
  std::unique_ptr<Map> map_;
  std::vector <std::shared_ptr<TTF_Font>> fonts_;
  std::vector <std::shared_ptr<OpenGlProgram>> opengl_programs_;
};