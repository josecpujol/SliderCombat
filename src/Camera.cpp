#include "Camera.h"
#include "GameObject.h"
#include "OpenGlResources.h"
#include <glm/gtc/matrix_transform.hpp>



void Camera::follow(GameObject* object, glm::vec3 eye_offset, glm::vec3 center_offset) {
  mode_ = CameraMode::Following;
  eye_offset_ = eye_offset;
  center_offset_ = center_offset;
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

void Camera::getMatrixFollowing(glm::mat4* mat) {
  const glm::vec3 pos = object_to_follow_->getPosition();
  float rot_z = object_to_follow_->getRotation();

  glm::vec3 eye_offset_rotated = glm::vec3(applyRotation(glm::vec2(eye_offset_), rot_z), eye_offset_.z);
  glm::vec3 center_offset_rotated = glm::vec3(applyRotation(glm::vec2(center_offset_), rot_z), center_offset_.z);

  *mat = glm::lookAt(pos + eye_offset_rotated, pos + center_offset_rotated, glm::vec3(0,0,1));
}

void Camera::getOpenglMatrix(glm::mat4* mat) {
  switch (mode_) {
  case CameraMode::Static:
    *mat = matrix_;
    break;
  case CameraMode::Following:
    getMatrixFollowing(mat);
    break;
  case CameraMode::Flying:
    break;
  }
}