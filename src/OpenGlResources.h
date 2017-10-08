#pragma once

#include <GL/glew.h>
#include "Math.h"


class OpenGlResources {
public:
  OpenGlResources() {}
  bool init();

  static void printShaderLog(GLuint shader);
  static void printProgramLog(GLuint program);
  static void drawAxis();
  static void drawCircle(float radius, int segments);
  static void drawSphere(float radius);
  static void drawVector(const glm::vec3& origin, const glm::vec3& end, const glm::vec3& color);
  static void drawVector(const glm::vec2& origin, const glm::vec2& end, const glm::vec3& color);
  static inline void setColor(const glm::vec3& color) {
    glColor3f(color.r, color.g, color.b);
  }


private:
  GLuint gProgramID_;
  GLint gVertexPos2DLocation = -1;
  GLuint gVBO = 0;
  GLuint gIBO = 0;
};
