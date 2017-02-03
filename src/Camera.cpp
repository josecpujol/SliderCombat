#include "Camera.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

void Camera::follow(GameObject* object, glm::vec3 offset) {
  mode_ = CameraMode::Following;
  object_to_follow_ = object;
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
  mode_ = CameraMode::Static;
  matrix_ = glm::lookAt(eye, center, up);
}

void Camera::apply() {
  glm::mat4 camera_matrix;
  getOpenglMatrix(&camera_matrix);
  glMultMatrixf(glm::value_ptr(camera_matrix));
}

void Camera::getMatrixFromObject(glm::mat4* mat) {
  const glm::vec3 pos = object_to_follow_->getPosition();
  float rot_z = object_to_follow_->getRotation();

  // Object "forward": local y axis
  *mat = glm::lookAt(pos + glm::vec3(0, -6, 3), pos + glm::vec3(0, 0, 2), glm::vec3(0,0,1));
}

void Camera::getOpenglMatrix(glm::mat4* mat) {
  switch (mode_) {
  case CameraMode::Static:
    *mat = matrix_;
    break;
  case CameraMode::Following:
    getMatrixFromObject(mat);
    break;
  case CameraMode::Flying:
    break;
  }
}