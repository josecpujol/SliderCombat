#include "graphics/OpenGlRenderer.h"

#include "graphics/Model3d.h"
#include "graphics/OpenGlProgram.h"
#include "graphics/Scene.h"
#include "utils/Stats.h"
#include "graphics/OpenGlState.h"

void OpenGlRenderer::render(Object3d* mesh, OpenGlProgram* ogl_program, Scene* scene) {
  ogl_program->use();

  // Load matrix
  ogl_program->setUniformMatrix4fv("u_MVPmatrix", OpenGlState::getInstance().getModelViewProjectionMatrix());

 // ogl_program->setUniform3fv("u_ambient_light", scene->getAmbientLight());

  GLint vertex_attrib_location  = ogl_program->getAttribLocation("a_position");
  GLint colors_attrib_location  = ogl_program->getAttribLocation("a_color");
 // GLint normals_attrib_location = ogl_program->getAttribLocation("a_normal");

  OpenGlResources::checkGlError();
  int num_triangles = mesh->getNumberTriangles();
  Stats::getInstance().num_objects++;
  Stats::getInstance().num_triangles += num_triangles;

  glEnableVertexAttribArray(vertex_attrib_location);
  glEnableVertexAttribArray(colors_attrib_location);
 // glEnableVertexAttribArray(normals_attrib_location);
  OpenGlResources::checkGlError();

  glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_vertices_buffer_.name);
  glVertexAttribPointer(vertex_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  OpenGlResources::checkGlError();

  glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_colors_buffer_.name);
  glVertexAttribPointer(colors_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  OpenGlResources::checkGlError();
  
  /*glBindBuffer(GL_ARRAY_BUFFER, mesh->ogl_normals_buffer_.name);
  glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
  OpenGlResources::checkGlError();
  */
  glDrawArrays(GL_TRIANGLES, 0, num_triangles * 3);
  //glDrawArrays(GL_POINTS, 0, num_triangles * 3);
  OpenGlResources::checkGlError();
}