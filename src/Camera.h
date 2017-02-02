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

  void follow(GameObject*, glm::vec3 offset_position);
  void lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
  // Applies the matrix to the opengl stack
  void apply();

private:
  void getOpenglMatrix(glm::mat4*);

private:
  enum class CameraMode {Static, Moving};
  CameraMode mode_ = CameraMode::Static;
  glm::mat4 matrix_;



};