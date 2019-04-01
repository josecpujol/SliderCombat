#include "OpenGlState.h"

void OpenGlState::useProgram(GLuint program_id) {
  if (program_in_use_ != program_id) {
    glUseProgram(program_id);
    OpenGlResources::checkGlError();
    program_in_use_ = program_id;
  }
}

void OpenGlState::pushMatrix() {
  active_stack_->push();
}

void OpenGlState::popMatrix() {
  active_stack_->pop();
}

void OpenGlState::loadIdentity() {
  glm::mat4 m(1.0);
  loadMatrix(m);
}

void OpenGlState::loadMatrix(const glm::mat4& m) {
  active_stack_->load(m);
}

void OpenGlState::multMatrix(const glm::mat4& m) {
  active_stack_->mult(m);
}

glm::mat4 OpenGlState::getModelViewProjectionMatrix() const {
  return projection_matrix_stack_.get() * modelview_matrix_stack_.get();
}

void OpenGlState::matrixMode(MatrixMode mode) {
  switch (mode) {
    case MatrixMode::kProjection:
      active_stack_ = &projection_matrix_stack_;
      return;
    case MatrixMode::kModelView:
      active_stack_ = &modelview_matrix_stack_;
      return;
  }
}