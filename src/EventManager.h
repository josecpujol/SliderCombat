#pragma once

#include <functional>
#include <map>
#include <vector>

#include "GameObject.h"
#include "Logger.h"


enum class EventType { Fire, RemoveObject };

class Event;
class EventManager {
public:
  EventManager() { }
  int Register(EventType, std::function<void(const Event&)>);
  void Unregister(int id);
  void notify(const Event& event);

private:
  // one level of indirection: ids, so we can unregister
  std::map < EventType, std::vector< int >> type_to_ids_map_;
  std::map <int, std::function<void(const Event&) >> id_to_callback_map_;
  int last_assigned_id_ = 0;
};

class Event {
public:
  Event(EventType type, GameObject* sender) : type_(type), sender_(sender) {}
  static EventManager manager;
  void send() {
    manager.notify(*this);
  }
  EventType getType() const { return type_; }
  GameObject* getSender() const { return sender_; }

private:
  EventType type_;
  GameObject* sender_ = nullptr;
};