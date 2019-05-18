#pragma once

#include "mathnphysics/Math.h"

class Object3d;
class OpenGlProgram;
class Scene;

class OpenGlRenderer {
public:
  void render(Object3d*, const glm::mat3& model_matrix, const glm::vec3& emissive_color, OpenGlProgram*, Scene*);
};