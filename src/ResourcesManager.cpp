#include "ResourcesManager.h"
#include "SDL.h"
#include "SDL_image.h"
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
  initJoystick();
  if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG)) {
    LOG_ERROR("Could not initialize img library");
    return false;
  }

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
  std::map<FontType, std::string> fonts_location = {
    {FontType::kPrototype, "prototype_32.fnt"}
  };

  std::string fonts_dir = ResourcesManager::getResourceBaseDirectory() + std::string("fonts/");

  for (auto font_location : fonts_location) {
    std::string path = fonts_dir + font_location.second;
    LOG_DEBUG("Loading font: " << path);
    std::shared_ptr<BitmapFont> bm_font = std::make_shared<BitmapFont>();
    if (!bm_font->load(path)) {
      return false;
    }

    bm_fonts_[font_location.first] = bm_font;
  }

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
    std::string path = model_dir + model_location.second;
    LOG_DEBUG("Loading model: " << path);
    if (!model->load(path)) {
      return false;
    }
    LOG_INFO("Model loaded correctly: " << model_location.second);
    models_[model_location.first] = std::move(model);
  }
  
  return true;
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

  bm_fonts_.clear();

  SDL_JoystickClose(joystick_);
  IMG_Quit();
}

void ResourcesManager::initJoystick() {
  if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) != 0) {
    LOG_ERROR("Could not initialize gamecontroller");
    return;
  }
  int num_joysticks = SDL_NumJoysticks();
  LOG_INFO("#joysticks found: " << num_joysticks);
  for (int i = 0; i < num_joysticks; i++) {
    LOG_INFO("Joystick " << i << ": " << SDL_JoystickNameForIndex(i));
    joystick_ = SDL_JoystickOpen(i);
    if (joystick_) {
      int joystick_num_buttons = SDL_JoystickNumButtons(joystick_);
      LOG_INFO("Joystick number of buttons: " << joystick_num_buttons);
      joystick_state_.buttons.resize(joystick_num_buttons);
      break;
    } else {
      LOG_ERROR("Could not open joystick. Error: " << SDL_GetError());
    }
  }
}

const JoystickState* ResourcesManager::getJoystickState() {
  if (!joystick_) {
    return nullptr;
  }
  for (int i = 0; i < joystick_state_.buttons.size(); i++) {
    joystick_state_.buttons[i] = SDL_JoystickGetButton(joystick_, i);
  }

  joystick_state_.up = false;
  joystick_state_.down = false;
  joystick_state_.right = false;
  joystick_state_.left = false;
  Sint16 axis0 = SDL_JoystickGetAxis(joystick_, 0);
  Sint16 axis1 = SDL_JoystickGetAxis(joystick_, 1);
  if (axis0 > 100) joystick_state_.right = true;
  if (axis0 < -100) joystick_state_.left = true;
  if (axis1 > 100) joystick_state_.down = true;
  if (axis1 < -100) joystick_state_.up = true;

  return &joystick_state_;
}
