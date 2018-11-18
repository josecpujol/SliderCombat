#pragma once

#include "Math.h"

struct Pose {
  Pose(const glm::vec3& pos, float rot) : position(pos), rotation(rot) {}
  Pose() = delete;
  glm::vec3 position;
  float rotation;
};