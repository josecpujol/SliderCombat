#pragma once

#include <functional>
#include <map>
#include <vector>

#include "GameObject.h"
#include "Logger.h"


class Event;
class RemoveObjectEvent;
class AddObjectEvent;
class EventManager {
public:
  EventManager() { }
  int subscribeToRemoveObjectEvent(std::function<void(GameObject*)>);
  int subscribeToAddObjectEvent(std::function<void(GameObject*)> callback);

  void unsubscribe(int id);
  void notifyRemoveObjectEvent(const RemoveObjectEvent& event);
  void notifyAddObjectEvent(const AddObjectEvent& event);

private:
  std::map <int, std::function<void(GameObject*)>> remove_object_callbacks_;
  std::map <int, std::function<void(GameObject*)>> add_object_callbacks_;

  int last_assigned_id_ = 0;
};

class Event {
public:
  Event() {}
  static EventManager manager;
  virtual void send() = 0;
};


class RemoveObjectEvent : public Event {
public:
  RemoveObjectEvent(GameObject* object_to_remove) : object_to_remove(object_to_remove) {}
  void send() override {
    manager.notifyRemoveObjectEvent(*this);
  }
  GameObject* object_to_remove = nullptr;
};

class AddObjectEvent : public Event {
public:
  AddObjectEvent(GameObject* object_to_add) :  object_to_add(object_to_add) {}
  void send() override {
    manager.notifyAddObjectEvent(*this);
  }
  GameObject* object_to_add = nullptr;
};