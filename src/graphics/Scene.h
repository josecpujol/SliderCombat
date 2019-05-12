#pragma once

#include "mathnphysics/Math.h"
#include "graphics/OpenGlRenderer.h"

class Object3dHolder;

class Scene {
public:
  Scene() = default;
  void setAmbientLight(const glm::vec3& color) { ambient_light_ = color; };
  glm::vec3 getAmbientLight() const { return ambient_light_; }
  void setDiffuseLight(const glm::vec4& position, const glm::vec3& diffuse_color);  // No specular for now
  void render(const Object3dHolder&);

private:
  glm::vec3 ambient_light_ = glm::vec3(1.0f);
  glm::vec3 diffuse_light_color_;
  glm::vec3 diffuse_light_position_;
  OpenGlRenderer ogl_renderer_;
};