#include "OpenGlResources.h"
#include "OpenGlState.h"

#include "utils/Logger.h"


#if defined(_DEBUG) || defined(DEBUG)
void OpenGlResources::checkGlError() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    LOG_DEBUG(err);
    assert(false);
  }
}
#else
void OpenGlResources::checkGlError() {}
#endif

void OpenGlResources::drawCircle(float radius, int num_segments, const glm::vec3& color) {
  std::vector<glm::vec2> vertices;
  vertices.resize(num_segments);

  for (int i = 0; i < num_segments; i++) {
    float theta = (2.0f * 3.1415926f * i) / num_segments;

    float x = radius * cosf(theta);
    float y = radius * sinf(theta);

    vertices[i] = glm::vec2(x, y);
  }

  OpenGlResources::drawPolygon(vertices, color);
}

void OpenGlResources::drawMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& colors, GLenum mode) {
  OpenGlBuffer vertices_buffer;
  OpenGlBuffer colors_buffer;

  OpenGlProgram* ogl_program = ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d);
  ogl_program->use();

  ogl_program->setUniformMatrix4fv("u_MVPmatrix", OpenGlState::getInstance().getModelViewProjectionMatrix());

  GLuint vertex_attrib_location = ogl_program->getAttribLocation("a_position");
  GLuint colors_attrib_location = ogl_program->getAttribLocation("a_color");

  glEnableVertexAttribArray(vertex_attrib_location);
  glEnableVertexAttribArray(colors_attrib_location);

  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer.name);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::remove_reference<decltype(vertices)>::type::value_type), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(vertex_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, colors_buffer.name);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(std::remove_reference<decltype(colors)>::type::value_type), &colors[0], GL_STATIC_DRAW);
  glVertexAttribPointer(colors_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(mode, 0, vertices.size());
  OpenGlResources::checkGlError();
}

void OpenGlResources::drawPolygon(const std::vector<glm::vec2>& vertices, const std::vector<glm::vec3>& colors, GLenum mode) {
  OpenGlBuffer vertices_buffer;
  OpenGlBuffer colors_buffer;

  OpenGlProgram* ogl_program = ResourcesManager::getInstance().getOpenGlProgram(OpenGlProgramType::kModel3d);
  ogl_program->use();

  ogl_program->setUniformMatrix4fv("u_MVPmatrix", OpenGlState::getInstance().getModelViewProjectionMatrix());

  GLuint vertex_attrib_location = ogl_program->getAttribLocation("a_position");
  GLuint colors_attrib_location = ogl_program->getAttribLocation("a_color");

  glEnableVertexAttribArray(vertex_attrib_location);
  glEnableVertexAttribArray(colors_attrib_location);

  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer.name);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(std::remove_reference<decltype(vertices)>::type::value_type), &vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(vertex_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, colors_buffer.name);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(std::remove_reference<decltype(colors)>::type::value_type), &colors[0], GL_STATIC_DRAW);
  glVertexAttribPointer(colors_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(mode, 0, vertices.size());
  OpenGlResources::checkGlError();
}

void OpenGlResources::drawPolygon(const std::vector<glm::vec2>& vertices, const glm::vec3& color) {
  OpenGlBuffer vertices_buffer;
  OpenGlBuffer colors_buffer;

  std::vector<glm::vec3> colors;
  colors.resize(vertices.size());
  std::fill(colors.begin(), colors.end(), color);

  OpenGlResources::drawPolygon(vertices, colors, GL_LINE_LOOP);
}


void OpenGlResources::drawAxis() {
  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f));
  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f));
}

void OpenGlResources::drawVector(const glm::vec3& origin, const glm::vec3& end, const glm::vec3& color) {
  std::vector<glm::vec3> vertices;

  vertices.push_back(origin);
  vertices.push_back(end);

  std::vector<glm::vec3> colors;
  colors.resize(vertices.size());
  std::fill(colors.begin(), colors.end(), color);

  OpenGlResources::drawMesh(vertices, colors, GL_LINES);
}

void OpenGlResources::drawVector(const glm::vec2& origin, const glm::vec2& end, const glm::vec3& color) {
  OpenGlResources::drawVector(glm::vec3(origin, 0), glm::vec3(end, 0), color);
}

bool OpenGlResources::init() {
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    LOG_ERROR("Error initializing GLEW! " << glewGetErrorString(glewError));
    return false;
  }
    
  return true;
}
