#include "ResourcesManager.h"
#include "SDL.h"
#include "Logger.h"
#include <experimental/filesystem>

void ResourcesManager::setWindowDimensions(int w, int h) {
  window_width_ = w;
  window_height_ = h;
}

void ResourcesManager::getWindowDimensions(int* w, int* h) {
  *w = window_width_;
  *h = window_height_;
}

std::string ResourcesManager::getResourceBaseDirectory() {
  std::string dir = std::experimental::filesystem::current_path().string();
  LOG_DEBUG("Base path: " << dir);
  dir += std::string("/data/");
  return dir;
}

bool ResourcesManager::loadMaps() {
  std::unique_ptr<Map> map = std::make_unique<Map>();
  std::string data_dir = ResourcesManager::getResourceBaseDirectory();
  data_dir += std::string("maps/");
  if (!map->load(data_dir + "/map2.tmx")) {
    return false;
  }
  map_ = std::move(map);
  return true;
}

Map* ResourcesManager::getMap() {
  return map_.get();
}

bool ResourcesManager::loadResources() {
  if (!loadModels()) {
    LOG_ERROR("Could not load models");
    return false;
  }
  if (!loadMaps()) {
    LOG_ERROR("Could not load maps");
    return false;
  }
  if (!loadFonts()) {
    LOG_ERROR("Could not load fonts");
  }
  return true;
}

bool ResourcesManager::loadFonts() {
  std::map<FontType, std::string> fonts_location = {
    {FontType::kRobotoCondensed, "RobotoCondensed-Regular.ttf"}
  };

  std::string fonts_dir = ResourcesManager::getResourceBaseDirectory() + std::string("fonts/");

  for (auto& font_location : fonts_location) {
    std::string path = fonts_dir + font_location.second;
    TTF_Font* font = TTF_OpenFont(path.c_str(), 20);
    if (!font) {
      return false;
    }
    LOG_INFO("Font loaded correctly: " << font_location.second);
    fonts_[font_location.first] = font;
  }
  return true;
}

bool ResourcesManager::loadModels() {
  std::map<ModelType, std::string> models_location = {
    {ModelType::kTiles, "tiles.obj"},
    {ModelType::kTank, "tank.obj"},
    {ModelType::kProjectiles, "projectiles.obj"}
  };

  std::string model_dir = ResourcesManager::getResourceBaseDirectory();
  model_dir += std::string("3dmodels/");
  for (auto& model_location : models_location) {
    std::unique_ptr<Model3d> model = std::make_unique<Model3d>();
    if (!model->load(model_dir + model_location.second)) {
      return false;
    }
    LOG_INFO("Model loaded correctly: " << model_location.second);
    models_[model_location.first] = std::move(model);
  }
  
  return true;
}

TTF_Font* ResourcesManager::getFont(FontType type) {
  if (fonts_.count(type) == 0) {
    assert(false && "Font does not exist");
    return nullptr;
  } else {
    return fonts_[type];
  }
}

Model3d* ResourcesManager::getModel3d(ModelType type) {
  if (models_.count(type) == 0) {
    assert(false && "Model does not exist");
    return nullptr;
  } else {
    return models_[type].get();
  }
}

void ResourcesManager::releaseResources() {
  for (auto& model : models_) {
    model.second = nullptr;
  }

  for (auto& font : fonts_) {
    TTF_CloseFont(font.second);
  }
}
