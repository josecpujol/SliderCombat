#include "OpenGlState.h"

void OpenGlState::useProgram(GLuint program_id) {
  if (program_in_use_ != program_id) {
    glUseProgram(program_id);
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
  load(m);
}

void OpenGlState::load(const glm::mat4& m) {
  active_stack_->load(m);
  glLoadMatrixf(glm::value_ptr(m));
}

void OpenGlState::mult(const glm::mat4& m) {
  active_stack_->mult(m);
  auto res = active_stack_->get();
  glLoadMatrixf(glm::value_ptr(res));
}

void OpenGlState::matrixMode(MatrixMode mode) {
  switch (mode) {
    case MatrixMode::kProjection:
      active_stack_ = &projection_matrix_stack_;
      glMatrixMode(GL_PROJECTION);
      return;
    case MatrixMode::kModelView:
      active_stack_ = &modelview_matrix_stack_;
      glMatrixMode(GL_MODELVIEW);
      return;
  }
}