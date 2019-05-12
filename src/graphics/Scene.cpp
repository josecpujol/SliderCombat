#include "graphics/Scene.h"

#include <cassert>

#include "graphics/Model3d.h"
#include "graphics/OpenGlResources.h"
#include "graphics/OpenGlState.h"
#include "graphics/OpenGlProgram.h"


void Scene::setDiffuseLight(const glm::vec4& position, const glm::vec3& diffuse_color) {
  diffuse_light_position_ = position;
  diffuse_light_color_ = diffuse_color;
}

void Scene::render(const Object3dHolder& object3d_holder) {
  
  OpenGlProgram* ogl_program = object3d_holder.getOpenGlProgram();
  Object3d* mesh = object3d_holder.getObject3d();
  assert(ogl_program);

  OpenGlState::getInstance().pushMatrix();
  OpenGlState::getInstance().multMatrix(object3d_holder.getTransformationMatrix());

  ogl_renderer_.render(mesh, ogl_program, this);
  
  OpenGlState::getInstance().popMatrix();
}
