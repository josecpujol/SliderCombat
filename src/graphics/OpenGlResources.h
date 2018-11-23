#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GL/glu.h>
#include "mathnphysics/Math.h"

class OpenGlProgram {
public:
  OpenGlProgram(const char* vertex_shader_source, const char* fragment_shader_source);
  ~OpenGlProgram();
  bool isCreated() const { return is_program_created_; }
  void use();
private:
  GLint program_id_;
  bool is_program_created_ = false;
};

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
  std::vector < std::shared_ptr<OpenGlProgram>> programs_;

};
