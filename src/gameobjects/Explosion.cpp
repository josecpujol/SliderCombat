#include "Explosion.h"

#include "EventManager.h"

Explosion::Explosion(const glm::vec3& pos, Duration duration, float max_speed, Object3d* particle) : 
  GameObject(GameObjectType::Explosion, pos, 0.f, 1.f) {
  total_duration_ = std::chrono::duration_cast<std::chrono::microseconds>(duration);
  particle_.setObject3d(particle);
  particle_.setOpenGlProgram(ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d));
  int num_particles = 20;
  for (int i = 0; i < num_particles; i++) {
    AnimatedPose pose;
    pose.pos_init = pos;
    pose.pos_end = pos + glm::sphericalRand<float>((total_duration_.count() * max_speed) / 1000000);  // TODO: adjust pos end to have different destinations
    pose.rotation_init = glm::linearRand(glm::vec3(0.f), glm::vec3(100.f));
    pose.rotation_end = glm::linearRand(glm::vec3(-100.f), glm::vec3(300.f));
    pose.scale_init = 0.2f;
    pose.scale_end = 0.05f;
    particles_poses_.push_back(pose);
  }
}

void Explosion::render() {
  for (auto& pose : particles_poses_) {
    particle_.setTranslation(pose.pos);
    particle_.setRotation(pose.rotation);
    particle_.setScale(glm::vec3(pose.scale));
    particle_.render(true);  // will save and restore the modelview matrices
  }
}

void Explosion::update(uint32_t elapsed_us) {
  duration_ += std::chrono::microseconds(elapsed_us);

  float t = (float)(duration_.count()) / total_duration_.count();
  for (auto &pose : particles_poses_) {
    pose.update(t);
  }

  if (duration_ > total_duration_) {
    RemoveObjectEvent event(this);
    event.send();
  }
}