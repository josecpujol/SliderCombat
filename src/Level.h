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

private:
  void onFire(const FireEvent& event);
  void addObject(GameObject*);
  void removeObject(GameObject*);
  void addPendingObjects();
  void removePendingObjects();
  void checkCollisions();
  void updateCameraSetup(const Uint8*);
  void updateRenderFlags(const Uint8*);
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

};