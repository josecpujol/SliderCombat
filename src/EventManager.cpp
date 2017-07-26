
#include "EventManager.h"

#include <cassert>

EventManager Event::manager = EventManager();

int EventManager::subscribeToRemoveObjectEvent(std::function<void(GameObject*)> callback) {
  int id = last_assigned_id_++;
  remove_object_callbacks_[id] = callback;
  return id;
}

int EventManager::subscribeToAddObjectEvent(std::function<void(GameObject*)> callback) {
  int id = last_assigned_id_++;
  add_object_callbacks_[id] = callback;
  return id;
}

void EventManager::unsubscribe(int id) {
  assert(false && "Implement");
}

void EventManager::notifyRemoveObjectEvent(const RemoveObjectEvent& event) {
  for (auto& entry : remove_object_callbacks_) {
    entry.second(event.object_to_remove);
  }
}

void EventManager::notifyAddObjectEvent(const AddObjectEvent& event) {
  for (auto& entry : add_object_callbacks_) {
    entry.second(event.object_to_add);
  }
}