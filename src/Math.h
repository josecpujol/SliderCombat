#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <math.h>

struct BoundVector3 {
  glm::vec3 origin;
  glm::vec3 direction;
};

struct BoundVector2 {
  BoundVector2() = default;
  BoundVector2(const glm::vec2& o, const glm::vec2& d) : origin(o), direction(d) {}
  glm::vec2 origin;
  glm::vec2 direction;
};


glm::vec2 applyRotation(const glm::vec2& input, float rot_deg);