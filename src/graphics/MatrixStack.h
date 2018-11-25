#pragma once

#include "mathnphysics/Math.h"
#include <array>

#define kNumLevels 32  // Opengl has up to 32 in some implementations

class MatrixStack {
public:
  void push();
  void pop();
  void load(const glm::mat4& m);
  void mult(const glm::mat4& m);
  const glm::mat4& get() const;

private:
  std::array<glm::mat4, kNumLevels> stack_;
  int active_index_ = 0;
};