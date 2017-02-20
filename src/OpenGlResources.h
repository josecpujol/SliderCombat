#pragma once


#include <GL/glew.h>

//#include <gl\GL.h>
//#include <gl\GLU.h>

class OpenGlResources {
public:
  OpenGlResources() {}
  bool init();

  static void printShaderLog(GLuint shader);
  static void printProgramLog(GLuint program);
  static void drawAxis();
  static void drawCircle(float radius, int segments);

private:
  GLuint gProgramID_;
  GLint gVertexPos2DLocation = -1;
  GLuint gVBO = 0;
  GLuint gIBO = 0;
};
