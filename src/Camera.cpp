#include "Camera.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

void Camera::follow(GameObject*, glm::vec3 offset) {

}

void Camera::lookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
  matrix_ = glm::lookAt(eye, center, up);
  mode_ = CameraMode::Static;
}

void Camera::apply() {
  glm::mat4 camera_matrix;
  getOpenglMatrix(&camera_matrix);
  glMultMatrixf(glm::value_ptr(camera_matrix));
}

void Camera::getOpenglMatrix(glm::mat4* mat) {
  switch (mode_) {
  case CameraMode::Static:
    *mat = matrix_;
    break;
  case CameraMode::Moving:
    break;
  }
}