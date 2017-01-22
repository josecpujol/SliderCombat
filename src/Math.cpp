#include "Math.h"
glm::vec2 applyRotation(glm::vec2 input, float rot_deg) {
  float rot_z_rad = glm::radians(rot_deg);
  float cos_rot = cos(rot_z_rad);
  float sin_rot = sin(rot_z_rad);
  // convert local positions to global
  return glm::mat2(cos_rot, sin_rot, -sin_rot, cos_rot) * input;
 // return glm::mat2(1, 2, 3, 4) * input;
}