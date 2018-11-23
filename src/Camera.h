#pragma once

#include "mathnphysics/Math.h"
#include "graphics/OpenGlResources.h"

class GameObject;

/*
 * Orientation: same as opengl. Considering looking through the screen
 * > x, ^y, oz (backwards)
 */
class Camera {
public:
  Camera() = default;

  // Points always towards the object direction
  void follow(GameObject*, 
    glm::vec3 eye_offset,  ///< eye offset wrt the object position
    glm::vec3 center_offset  ///< center offset wrt the object position
  );

  // rotates around center
  void testMode();

  void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
  // Applies the matrix to the opengl stack
  void apply();
  void applyRotationOnly();

private:
  void getOpenglMatrix(glm::mat4*);
  void getMatrixFollowing(glm::mat4*);

private:
  enum class CameraMode {Static, Rotating, Flying, Following};
  CameraMode mode_ = CameraMode::Static;
  glm::mat4 matrix_;

  glm::vec3 eye_offset_;  // when following
  float angle_ = 0.f;  // when rotating
  glm::vec3 center_offset_;
  GameObject* object_to_follow_ = nullptr;
};