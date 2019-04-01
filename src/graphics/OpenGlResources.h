#pragma once

#include <memory>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GL/glu.h>
#include "mathnphysics/Math.h"



class OpenGlBuffer {
public:
  OpenGlBuffer() {
    glGenBuffers(1, &name);
  }
  ~OpenGlBuffer() {
    glDeleteBuffers(1, &name);
  }

  GLuint name = 0;
};

class OpenGlResources {
public:
  OpenGlResources() {}
  bool init();

  static void checkGlError();

  static void printShaderLog(GLuint shader);
  static void printProgramLog(GLuint program);
  static void drawAxis();
  static void drawCircle(float radius, int segments, const glm::vec3& color = glm::vec3(0.f, 0.f, 0.5f));
  static void drawPolygon(const std::vector<glm::vec2>& vertices , const glm::vec3& color = glm::vec3(0.f, 0.f, 0.5f));
  static void drawSphere(float radius);
  static void drawVector(const glm::vec3& origin, const glm::vec3& end, const glm::vec3& color);
  static void drawVector(const glm::vec2& origin, const glm::vec2& end, const glm::vec3& color);
};
