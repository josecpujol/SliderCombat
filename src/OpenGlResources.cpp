#include "OpenGlResources.h"

#include "utils/Logger.h"


OpenGlProgram::OpenGlProgram(const char* vertex_shader_source, const char* fragment_shader_source) {
  GLint shader_compiled;
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader_id, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertex_shader_id);
  shader_compiled = GL_FALSE;
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &shader_compiled);
  if (!shader_compiled) {
    LOG_ERROR("Unable to compile vertex shader: " << vertex_shader_id);
    GLint max_length = 0;
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &max_length);

    // The maxLength includes the NULL character
    std::vector<GLchar> error_log(max_length);
    glGetShaderInfoLog(vertex_shader_id, max_length, &max_length, &error_log[0]);
    return;
  }

  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader_id, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragment_shader_id);
  shader_compiled = GL_FALSE;
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &shader_compiled);
  if (!shader_compiled) {
    LOG_ERROR("Unable to compile shader: " << fragment_shader_id);
    GLint max_length = 0;
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &max_length);

    // The maxLength includes the NULL character
    std::vector<GLchar> error_log(max_length);
    glGetShaderInfoLog(vertex_shader_id, max_length, &max_length, &error_log[0]);
    return;
  }

  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vertex_shader_id);
  glAttachShader(program_id_, fragment_shader_id);
  glLinkProgram(program_id_);

  //Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint is_linked = 0;
  glGetProgramiv(program_id_, GL_LINK_STATUS, (int *)&is_linked);
  if (!is_linked) {
    LOG_ERROR("Could not link program: " << program_id_);

    //We don't need the program anymore.
    glDeleteProgram(program_id_);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return;
  }

  glDetachShader(program_id_, vertex_shader_id);
  glDetachShader(program_id_, fragment_shader_id);

  is_program_created_ = true;
}

OpenGlProgram::~OpenGlProgram() {
  if (isCreated()) {
    glDeleteProgram(program_id_);
  }
}

void OpenGlProgram::use() {
  assert(isCreated());
  glUseProgram(program_id_);
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

  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f));
  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
  drawVector(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.f, 0.f, 1.f));
}

void OpenGlResources::drawVector(const glm::vec3& origin, const glm::vec3& end, const glm::vec3& color) {
  GLboolean is_lighting_enabled = glIsEnabled(GL_LIGHTING);
  if (is_lighting_enabled) glDisable(GL_LIGHTING);
  glColor3f(color.r, color.g, color.b);
  glBegin(GL_LINES);
  glVertex3f(origin.x, origin.y, origin.z); 
  glVertex3f(end.x, end.y, end.z);
  glEnd();
  if (is_lighting_enabled) glEnable(GL_LIGHTING);
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
