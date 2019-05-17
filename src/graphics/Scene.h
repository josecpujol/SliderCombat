#pragma once

#include "mathnphysics/Math.h"
#include "graphics/OpenGlRenderer.h"

class Object3dHolder;

struct DiffuseLight {
  glm::vec4 position;
  glm::vec3 color = glm::vec3(0.0);
};

class Scene {
public:
  Scene() = default;
  void setAmbientLight(const glm::vec3& color) { ambient_light_ = color; };
  glm::vec3 getAmbientLight() const { return ambient_light_; }
  
  void setDiffuseLight(const DiffuseLight& diffuse_light);  // No specular for now
  DiffuseLight getDiffuseLight() const;
  bool isDiffuseLightEnabled() const;
  void enableDiffuseLight(bool enable);
  void render(const Object3dHolder&);

private:
  glm::vec3 ambient_light_ = glm::vec3(1.0f);
  DiffuseLight diffuse_light_;
  bool enable_diffuse_light_{ false };
  OpenGlRenderer ogl_renderer_;
};