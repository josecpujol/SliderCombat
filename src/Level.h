#pragma once

#include <vector>
#include <memory>

#include "SDL.h"
#include "GameObject.h"
//#include "EventListener.h"

class Event;
class Level {
public:
  Level();
  void render();
  void update(const Uint8*, uint32_t elapsed_us);
  void onEvent(const Event& event);

private:
  void addObject(GameObject*);
  void removeObject(GameObject*);
  void addObjects();
  void removeObjects();
  void checkCollisions();
  inline static void checkCollision(GameObject* obj1, GameObject* obj2);
  int loop_count_ = 0;
  std::vector < std::shared_ptr< GameObject> > objects_;
  std::vector < std::shared_ptr< GameObject> > objects_to_add_;  // need to add objects outside the update loop
  std::vector < GameObject* > objects_to_remove_;  // need to add objects outside the update loop


};