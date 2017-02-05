#pragma once

#include "Math.h"

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

  void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
  // Applies the matrix to the opengl stack
  void apply();

private:
  void getOpenglMatrix(glm::mat4*);
  void getMatrixFollowing(glm::mat4*);

private:
  enum class CameraMode {Static, Flying, Following};
  CameraMode mode_ = CameraMode::Static;
  glm::mat4 matrix_;

  glm::vec3 eye_offset_;  // when following
  glm::vec3 center_offset_;
  GameObject* object_to_follow_ = nullptr;
};