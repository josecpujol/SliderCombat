#include "graphics/OpenGlRenderer.h"

#include "graphics/Model3d.h"
#include "graphics/OpenGlProgram.h"
#include "graphics/Scene.h"
#include "utils/Stats.h"
#include "graphics/OpenGlState.h"
#include "utils/Logger.h"

void OpenGlRenderer::render(Object3d* mesh, const glm::mat3& model_matrix, OpenGlProgram* ogl_program, Scene* scene) {
  ogl_program->use();

 // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // Load matrix
  ogl_program->setUniformMatrix4fv("u_MVPmatrix", OpenGlState::getInstance().getModelViewProjectionMatrix());

  if (ogl_program->hasUniform("u_ambient_color")) {
    ogl_program->setUniform3fv("u_ambient_color", scene->getAmbientLight());
  }
  if (ogl_program->hasUniform("u_modelMatrix")) {
    ogl_program->setUniformMatrix3fv("u_modelMatrix", model_matrix);
  }
  if (ogl_program->hasUniform("u_normalMatrix")) {
    ogl_program->setUniformMatrix3fv("u_normalMatrix", glm::transpose(glm::inverse(model_matrix)));  // TODO: precalculate?
  }

  DiffuseLight diffuse_light;

  if (scene->isDiffuseLightEnabled()) {
    diffuse_light = scene->getDiffuseLight();
    if (ogl_program->hasUniform("u_diffuse_pos")) {
      ogl_program->setUniform4fv("u_diffuse_pos", diffuse_light.position);
    }
  }
  if (ogl_program->hasUniform("u_diffuse_color")) {
    ogl_program->setUniform3fv("u_diffuse_color", diffuse_light.color);
  }


  GLint vertex_attrib_location  = ogl_program->getAttribLocation("a_position");
  GLint colors_attrib_location  = ogl_program->getAttribLocation("a_color");
  GLint normals_attrib_location;
  if (scene->isDiffuseLightEnabled()) {
    normals_attrib_location = ogl_program->getAttribLocation("a_normal");
  }

  OpenGlResources::checkGlError();
  int num_triangles = mesh->getNumberTriangles();
  Stats::getInstance().num_objects++;
  Stats::getInstance().num_triangles += num_triangles;

  glEnableVertexAttribArray(vertex_attrib_location);
  glEnableVertexAttribArray(colors_attrib_location);
  if (scene->isDiffuseLightEnabled()) {
    glEnableVertexAttribArray(normals_attrib_location);
  }
  OpenGlResources::checkGlError();

  glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_vertices_buffer_.name);
  glVertexAttribPointer(vertex_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  OpenGlResources::checkGlError();

  glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_colors_buffer_.name);
  glVertexAttribPointer(colors_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  OpenGlResources::checkGlError();
  
  if (scene->isDiffuseLightEnabled()) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_normals_buffer_.name);
    glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    OpenGlResources::checkGlError();
  }
  
  glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);
  OpenGlResources::checkGlError();
}