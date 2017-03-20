#include "OpenGlResources.h"

#include <math.h>

#include "Logger.h"


void OpenGlResources::printProgramLog(GLuint program) {

}

void OpenGlResources::printShaderLog(GLuint shader) {

}

void OpenGlResources::drawCircle(float radius, int num_segments) {
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < num_segments; i++) {
    float theta = (2.0f * 3.1415926f * i) / num_segments;

    float x = radius * cosf(theta);
    float y = radius * sinf(theta);

    glVertex2f(x, y);//output vertex
  }
  glEnd();
}


void OpenGlResources::drawAxis() {
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
  glColor3f(0, 1, 0);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0, 0, 1);
  glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
  glEnd();
}

bool OpenGlResources::init() {
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK) {
    LOG_ERROR("Error initializing GLEW! " << glewGetErrorString(glewError));
    return false;
  }

  //Generate program
  gProgramID_ = glCreateProgram();

/*  //Create vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vertexShaderSource[] =
  {
    "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
  };

  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLint vShaderCompiled = GL_FALSE;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
  if (vShaderCompiled != GL_TRUE) {
    LOG_ERROR("Unable to compile vertex shader " << vertexShader);
    printShaderLog(vertexShader);
    return false;
  }

  glAttachShader(gProgramID_, vertexShader);

  //Create fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fragmentShaderSource[] =
  {
    "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
  };

  glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  GLint fShaderCompiled = GL_FALSE;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
  if (fShaderCompiled != GL_TRUE) {
    LOG_ERROR("Unable to compile fragment shader " << fragmentShader);
    printShaderLog(fragmentShader);
    return false;
  }

  glAttachShader(gProgramID_, fragmentShader);


  //Link program
  glLinkProgram(gProgramID_);

  //Check for errors
  GLint programSuccess = GL_TRUE;
  glGetProgramiv(gProgramID_, GL_LINK_STATUS, &programSuccess);
  if (programSuccess != GL_TRUE) {
    LOG_ERROR("Error linking program: " << gProgramID_);
    printProgramLog(gProgramID_);
    return false;
  }

  //Get vertex attribute location
  gVertexPos2DLocation = glGetAttribLocation(gProgramID_, "LVertexPos2D");
  if (gVertexPos2DLocation == -1) {
    LOG_ERROR("LVertexPos2D is not a valid glsl program variable!\n");
    return false;
  }
  //Initialize clear color
  glClearColor(0.f, 0.f, 0.f, 1.f);

  //VBO data
  GLfloat vertexData[] =
  {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f,  0.5f,
    -0.5f,  0.5f
  };

  //IBO data
  GLuint indexData[] = {0, 1, 2, 3};

  //Create VBO
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

  //Create IBO
  glGenBuffers(1, &gIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
  */
  return true;
}
