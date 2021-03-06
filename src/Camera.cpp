#include "Camera.h"
#include "gameobjects/GameObject.h"
#include "mathnphysics/Math.h"
#include "graphics/OpenGlState.h"


void Camera::follow(GameObject* object, glm::vec3 eye_offset, glm::vec3 center_offset) {
  mode_ = CameraMode::Following;
  eye_offset_ = eye_offset;
  center_offset_ = center_offset;
  object_to_follow_ = object;
}

void Camera::testMode() {
  mode_ = CameraMode::Rotating;
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
  mode_ = CameraMode::Static;
  matrix_ = glm::lookAt(eye, center, up);
}

void Camera::apply() {
  glm::mat4 camera_matrix;
  getOpenglMatrix(&camera_matrix);
  OpenGlState::getInstance().multMatrix(camera_matrix);
}

void Camera::applyRotationOnly() {
  glm::mat4 camera_matrix;
  getOpenglMatrix(&camera_matrix);
  // Remove translation
  camera_matrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  OpenGlState::getInstance().multMatrix(camera_matrix);
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
  case CameraMode::Rotating:
    angle_ += 0.01f;
    {
      glm::vec3 eye = glm::vec3(50.f, 50.f, 5.f);
      glm::vec3 center = eye + glm::vec3(20 * sin(angle_), 20 * cos(angle_), -5.f);

      *mat = glm::lookAt(eye, center, glm::vec3(0.f, 0.f, 1.f));
    }
    break;
  case CameraMode::Flying:
    break;
  }
}
