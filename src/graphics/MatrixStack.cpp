#include "MatrixStack.h"

void MatrixStack::push() {
  assert(active_index_ < (stack_.size() - 1));
  stack_[active_index_ + 1] = stack_[active_index_];
  active_index_++;
}

void MatrixStack::pop() {
  assert(active_index_ > 0);
  active_index_--;
}

void MatrixStack::load(const glm::mat4& m) {
  stack_[active_index_] = m;
}

void MatrixStack::mult(const glm::mat4& m) {
  stack_[active_index_] *= m;
}

const glm::mat4& MatrixStack::get() const {
  return stack_.at(active_index_);
}