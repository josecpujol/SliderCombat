
#include "EventManager.h"

#include <cassert>

EventManager Event::manager = EventManager();

int EventManager::Register(EventType type, std::function<void(const Event&)> callback) {
  int id = last_assigned_id_++;
  type_to_ids_map_[type].push_back(id);
  id_to_callback_map_[id] = callback;
  return id;
}

void EventManager::Unregister(int id) {
  assert(false && "Implement");
}

void EventManager::notify(const Event& event) {
  if (type_to_ids_map_.count(event.getType()) == 0) {
    return;
  }
  auto& ids = type_to_ids_map_[event.getType()];
  for (auto id : ids) {
    id_to_callback_map_[id](event);
  }
}