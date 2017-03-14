#pragma once

#include <vector>
#include <memory>

#include "SDL.h"
#include "GameObject.h"
#include "EventManager.h"
#include "Camera.h"
#include "Slider.h"
//#include "EventListener.h"

class Map;

class Level {
public:
  Level(Map*);
  void render();
  void update(const Uint8*, uint32_t elapsed_us);
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

private:
  void onFire(const FireEvent& event);
  void addObject(GameObject*);
  void removeObject(GameObject*);
  void addPendingObjects();
  void removePendingObjects();
  void checkCollisions();
  void updateCameraSetup(const Uint8*);
  void updateRenderFlags(const Uint8*);
  void setOpenGlLights();
  inline static void checkCollision(GameObject* obj1, GameObject* obj2);
  int loop_count_ = 0;
  std::vector < std::shared_ptr< GameObject> > objects_;
  std::vector < std::shared_ptr< GameObject> > objects_to_add_;  // need to add objects outside the update loop
  std::vector < GameObject* > objects_to_remove_;  // need to add objects outside the update loop
  Map* map_ = nullptr;
  Camera camera_;
  std::shared_ptr<SliderLocalPlayer> local_player_;
  bool render_collision_area_ = false;
  bool render_objects_ = true;
  TimePoint last_time_render_state_changed_ = Clock::now();

  float ambient_coeff_ = 0.5;
  float diffuse_coeff_ = 0.5;
  struct PointWithTimer {
    glm::vec2 point;
    TimePoint expiration;
  };
  std::vector<PointWithTimer> collision_points_;

};