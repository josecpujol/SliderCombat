
#include "EventManager.h"

#include <cassert>

EventManager Event::manager = EventManager();

int EventManager::subscribeToFireEvent(std::function<void(const FireEvent&)> callback) {
  int id = last_assigned_id_++;
  fire_callbacks_[id] = callback;
  return id;
}

int EventManager::subscribeToRemoveObjectEvent(std::function<void(GameObject*)> callback) {
  int id = last_assigned_id_++;
  remove_object_callbacks_[id] = callback;
  return id;
}

void EventManager::unsubscribe(int id) {
  assert(false && "Implement");
}

void EventManager::notifyFireEvent(const FireEvent& event) {
  for (auto& entry : fire_callbacks_) {
    entry.second(event);
  }
}

void EventManager::notifyRemoveObjectEvent(const RemoveObjectEvent& event) {
  for (auto& entry : remove_object_callbacks_) {
    entry.second(event.getSender());
  }
}