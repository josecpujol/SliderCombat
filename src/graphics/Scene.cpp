#include "graphics/Scene.h"

#include <cassert>

#include "graphics/Model3d.h"
#include "graphics/OpenGlResources.h"
#include "graphics/OpenGlState.h"
#include "graphics/OpenGlProgram.h"


void Scene::setDiffuseLight(const DiffuseLight& light) {
  diffuse_light_ = light;
}

DiffuseLight Scene::getDiffuseLight() const {
  return diffuse_light_;
}

bool Scene::isDiffuseLightEnabled() const {
  return enable_diffuse_light_;
}

void Scene::enableDiffuseLight(bool enable) {
  enable_diffuse_light_ = enable;
}

void Scene::render(const Object3dHolder& object3d_holder) {
  
  OpenGlProgram* ogl_program = object3d_holder.getOpenGlProgram();
  Object3d* mesh = object3d_holder.getObject3d();
  assert(ogl_program);

  OpenGlState::getInstance().pushMatrix();
  OpenGlState::getInstance().multMatrix(object3d_holder.getTransformationMatrix());

  ogl_renderer_.render(mesh, object3d_holder.getTransformationMatrix(), object3d_holder.getEmissiveColor(), ogl_program, this);
  
  OpenGlState::getInstance().popMatrix();
}
