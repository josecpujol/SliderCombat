#pragma once

#include <functional>
#include <map>
#include <vector>

#include "GameObject.h"
#include "Logger.h"


enum class EventType { Fire, RemoveObject };

class Event;
class FireEvent;
class RemoveObjectEvent;
class EventManager {
public:
  EventManager() { }
  int subscribeToFireEvent(std::function<void(const FireEvent&)>);
  int subscribeToRemoveObjectEvent(std::function<void(GameObject*)>);

  void unsubscribe(int id);
  void notifyFireEvent(const FireEvent& event);
  void notifyRemoveObjectEvent(const RemoveObjectEvent& event);

private:
  std::map <int, std::function<void(const FireEvent&)>> fire_callbacks_;
  std::map <int, std::function<void(GameObject*)>> remove_object_callbacks_;

  int last_assigned_id_ = 0;
};

class Event {
public:
  Event(EventType type, GameObject* sender) : type_(type), sender_(sender) {}
  static EventManager manager;
  virtual void send() = 0;
  EventType getType() const { return type_; }
  GameObject* getSender() const { return sender_; }

private:
  EventType type_;
  GameObject* sender_ = nullptr;
};

class FireEvent : public Event {
public:
  FireEvent(GameObject* sender, const glm::vec3& pos, float rot_z) : 
    Event(EventType::Fire, sender), pos_(pos), rot_z_(rot_z) {}
  void send() override {
    manager.notifyFireEvent(*this);
  }
  glm::vec3 pos_;
  float rot_z_;
};

class RemoveObjectEvent : public Event {
public:
  RemoveObjectEvent(GameObject* sender) : Event(EventType::RemoveObject, sender) {}
  void send() override {
    manager.notifyRemoveObjectEvent(*this);
  }
};