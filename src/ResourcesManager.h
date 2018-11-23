#pragma once

#include <memory>
#include "Map.h"
#include "SDL_joystick.h"
#include "graphics/BitmapFont.h"
#include "graphics/OpenGlResources.h"
#include "graphics/Model3d.h"

enum class ModelType {
  kTiles,
  kTank,
  kProjectiles,
  kPowerUps
};

enum class FontType {
  kPrototype
};

enum class OpenGlPrograms {
  kBasic
};

struct JoystickState {
  bool up = false;
  bool down = false;
  bool right = false;
  bool left = false;
  std::vector<bool> buttons;
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
  BitmapFont* getBitmapFont(FontType type) { return bm_fonts_[type].get(); }
  Map* getMap();
  const JoystickState* getJoystickState();

    
private:
  ResourcesManager() {}

  bool loadModels();
  bool loadMaps();
  bool loadFonts();
  bool loadOpenGlPrograms();
  void initJoystick();
  
  int window_width_ = 0;
  int window_height_ = 0;
  std::map<ModelType, std::unique_ptr<Model3d>> models_;
  std::unique_ptr<Map> map_;
  std::map<FontType, std::shared_ptr<BitmapFont>> bm_fonts_;

  std::vector <std::shared_ptr<OpenGlProgram>> opengl_programs_;
  SDL_Joystick* joystick_ = nullptr;
  int joystick_num_buttons_ = 0;
  JoystickState joystick_state_;
};