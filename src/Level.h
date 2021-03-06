#pragma once

#include <vector>
#include <memory>

#include "SDL.h"
#include "gameobjects/GameObject.h"
#include "EventManager.h"
#include "Camera.h"
#include "gameobjects/Slider.h"
#include "graphics/Scene.h"
#include "graphics/Model3d.h"
#include "Hud.h"
//#include "EventListener.h"

class Map;

class Level {
public:
  Level(Map*);
  void render();
  void update(uint32_t elapsed_us);
 /* void incAmbient(float inc) {
    ambient_coeff_ += inc;
    ambient_coeff_ = std::max(0.0f, ambient_coeff_);
    ambient_coeff_ = std::min(1.0f, ambient_coeff_);
    LOG_DEBUG("ambient: " << ambient_coeff_);
  }
  void incDiffuse(float inc) {
    diffuse_coeff_ += inc;
    diffuse_coeff_ = std::max(0.0f, diffuse_coeff_);
    diffuse_coeff_ = std::min(1.0f, diffuse_coeff_);
    LOG_DEBUG("diffuse: " << diffuse_coeff_);
  }*/

  std::shared_ptr<const SliderLocalPlayer> getLocalPlayer() { return local_player_; }
private:
  void addObject(GameObject*);
  void removeObject(GameObject*);
  void addPendingObjects();
  void removePendingObjects();
  void checkCollisions();
  void updateCameraSetup(const Uint8*);
  void updateRenderFlags(const Uint8*);
  void setOpenGlLights();
  void drawSkyDome();
  void createDomeMesh();
  inline static void checkCollision(GameObject* obj1, GameObject* obj2);
  std::vector < std::shared_ptr< GameObject> > objects_;
  std::vector < std::shared_ptr< GameObject> > objects_to_add_;  // need to add objects outside the update loop
  std::vector < GameObject* > objects_to_remove_;  // need to add objects outside the update loop
  std::unique_ptr<Hud> hud_;
  Map* map_ = nullptr;
  Camera camera_;
  std::shared_ptr<SliderLocalPlayer> local_player_;
  bool render_collision_area_ = false;
  bool render_objects_ = true;
  TimePoint last_time_render_state_changed_ = Clock::now();

  float ambient_coeff_ = 0.5;
  float diffuse_coeff_ = 0.5;
  struct VectorWithTimer {
    BoundVector2 bound_vector;
    TimePoint expiration;
  };
  std::vector<VectorWithTimer> collision_points_;

  Scene scene_;

  // TODO: move this somewhere...
  Object3dHolder dome_holder_;
  Object3d dome_mesh_{ std::string("dome") };

};