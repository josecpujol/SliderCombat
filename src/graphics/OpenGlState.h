#pragma once

#include "OpenGlResources.h"

#include "MatrixStack.h"

enum class MatrixMode { kProjection, kModelView };

class OpenGlState {
public:
  OpenGlState(OpenGlState const&) = delete;
  void operator=(OpenGlState const&) = delete;

  static OpenGlState& getInstance() {
    static OpenGlState instance;
    return instance;
  }

  void useProgram(GLuint);

  // Implementation of pushmatrix and popmatrix
  void pushMatrix();
  void popMatrix();
  void matrixMode(MatrixMode);
  void loadIdentity();
  void load(const glm::mat4& m);
  void mult(const glm::mat4& m);

private:
  OpenGlState() = default;
  GLuint program_in_use_ = 0;  // invalid program id
  
  MatrixStack projection_matrix_stack_;
  MatrixStack modelview_matrix_stack_;
  MatrixStack* active_stack_ = nullptr;
};