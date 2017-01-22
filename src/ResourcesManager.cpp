#include "ResourcesManager.h"
#include "SDL.h"
#include "Logger.h"

void ResourcesManager::setWindowDimensions(int w, int h) {
  window_width_ = w;
  window_height_ = h;
}

void ResourcesManager::getWindowDimensions(int* w, int* h) {
  *w = window_width_;
  *h = window_height_;
}

bool ResourcesManager::loadModels() {
  std::map<ModelType, std::string> models_location = {
    {ModelType::kCube, "cube.obj"},
    {ModelType::kVio, "vio.obj"},
    {ModelType::kTank, "tank.obj"}
  };

  LOG_DEBUG("Base path: " << SDL_GetBasePath());
  std::string model_dir(SDL_GetBasePath());
  model_dir += std::string("..\\..\\data\\3dmodels\\");
  for (auto& model_location : models_location) {
    std::unique_ptr<Model3d> model = std::make_unique<Model3d>();
    if (!model->load(model_dir + model_location.second)) {
      return false;
    }
    models_[model_location.first] = std::move(model);
  }
  
  return true;
}

Model3d* ResourcesManager::getModel3d(ModelType type) {
  if (models_.count(type) == 0) {
    return nullptr;
  } else {
    return models_[type].get();
  }
}