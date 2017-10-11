#include "ResourcesManager.h"
#include "SDL.h"
#include "./Logger.h"

void ResourcesManager::setWindowDimensions(int w, int h) {
  window_width_ = w;
  window_height_ = h;
}

void ResourcesManager::getWindowDimensions(int* w, int* h) {
  *w = window_width_;
  *h = window_height_;
}

std::string ResourcesManager::getResourceBaseDirectory() {
  return "./data/";
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

bool ResourcesManager::loadOpenGlPrograms() {

  std::string vertex_shader = "\
    varying vec4 vColor;\
    void main() {\
    vColor = gl_Color;\
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
  }";

  std::string fragment_shader = "\
    varying vec4 vColor;\
    void main() {\
    gl_FragColor = vColor;\
  }";
  std::shared_ptr<OpenGlProgram> program = std::make_shared<OpenGlProgram>(vertex_shader.c_str(), fragment_shader.c_str());
  if (!program->isCreated()) {
    LOG_ERROR("Error creating shader");
    return false;
  }
 // program->use();
  opengl_programs_.push_back(program);
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
    return false;
  }
  if (!loadOpenGlPrograms()) {
    LOG_ERROR("Could not load shaders");
    return false;
  }
  return true;
}

// See getfont
bool ResourcesManager::loadFonts() {
  return true;
}

bool ResourcesManager::loadModels() {
  std::map<ModelType, std::string> models_location = {
    {ModelType::kTiles, "tiles.obj"},
    {ModelType::kTank, "tank.obj"},
    {ModelType::kProjectiles, "projectiles.obj"},
    {ModelType::kPowerUps, "powerups.obj"}
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

// We load the fonts on the fly, as we need the point size
TTF_Font* ResourcesManager::getFont(FontType type, int point_size) {
  std::map<FontType, std::string> fonts_location = {
    {FontType::kRobotoCondensed, "RobotoCondensed-Regular.ttf"},
    {FontType::kPrototype, "Prototype.ttf"}
  };

  std::string fonts_dir = ResourcesManager::getResourceBaseDirectory() + std::string("fonts/");
  std::string path = fonts_dir + fonts_location[type];
  TTF_Font* font = TTF_OpenFont(path.c_str(), point_size);
  if (!font) {
    assert(false);
    return nullptr;
  }
  fonts_.push_back(std::shared_ptr<TTF_Font>(font, TTF_CloseFont));
  return font;
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

  fonts_.clear();
}
