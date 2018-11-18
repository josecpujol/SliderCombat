#include "Math.h"

// Assuming CoM is in (0, 0)
class Force {
public:
  Force(const BoundVector2& force_vector) : force_vector_(force_vector) {
    glm::vec3 v1 = glm::vec3(force_vector.origin, 0);
    glm::vec3 v2 = glm::vec3(force_vector.direction, 0);
    torque_ = (glm::cross(v1, v2)).z;
  }

  glm::vec2 getForceFreeVector() const { return force_vector_.direction; }
  float getTorque() const { return torque_; }
private:
  BoundVector2 force_vector_;
  float torque_ = 0.f;  // precompute cross vector
};