#include "Math.h"
glm::vec2 applyRotation(const glm::vec2& input, float rot_deg) {
  return glm::rotate(input, glm::radians(rot_deg));
}