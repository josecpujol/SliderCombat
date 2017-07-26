#include "Explosion.h"

#include "EventManager.h"

Explosion::Explosion(const glm::vec3& pos, Duration duration, float speed) : 
  GameObject(GameObjectType::Explosion, pos, 0.f, 1.f), speed_(speed) {
  total_duration_ = std::chrono::duration_cast<std::chrono::microseconds>(duration);
  LOG_DEBUG("Explosion constructor");
}

void Explosion::render() {

}

void Explosion::update(const Uint8* keys, uint32_t elapsed_us) {
  duration_ += std::chrono::microseconds(elapsed_us);
  if (duration_ > total_duration_) {
    RemoveObjectEvent event(this);
    event.send();
  }
}