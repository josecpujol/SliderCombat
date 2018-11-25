#include "OpenGlResources.h"
#include "OpenGlState.h"

#include "utils/Logger.h"

OpenGlProgram::~OpenGlProgram() {
  if (is_program_created_) {
    glDeleteProgram(program_id_);
  }
}

bool OpenGlProgram::load(const std::string& vertex_shader_source, const std::string& fragment_shader_source) {
  assert(!is_program_created_);
  GLuint vertex_shader_id = loadShader(vertex_shader_source, GL_VERTEX_SHADER);
  GLuint fragment_shader_id = loadShader(fragment_shader_source, GL_FRAGMENT_SHADER);
     
  program_id_ = glCreateProgram();
  glAttachShader(program_id_, vertex_shader_id);
  glAttachShader(program_id_, fragment_shader_id);
  glLinkProgram(program_id_);

  //Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint is_linked = 0;
  glGetProgramiv(program_id_, GL_LINK_STATUS, (int *)&is_linked);
  if (!is_linked) {
    LOG_ERROR("Could not link program: " << program_id_);

    glDeleteProgram(program_id_);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return false;
  }

  glDetachShader(program_id_, vertex_shader_id);
  glDetachShader(program_id_, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  is_program_created_ = true;
  return true;
}

GLuint OpenGlProgram::loadShader(const std::string& source, GLenum type) {
  GLint shader_compiled;
	GLuint shader_id = glCreateShader(type);
  const char* c_str = source.c_str();
	glShaderSource(shader_id, 1, &c_str, nullptr);
	glCompileShader(shader_id);
	shader_compiled = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);
	if (!shader_compiled) {
		LOG_ERROR("Unable to compile shader type: " << type << ", id: " << shader_id);
		GLint max_length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

		// The maxLength includes the NULL character
		std::vector<GLchar> error_log(max_length);
		glGetShaderInfoLog(shader_id, max_length, &max_length, &error_log[0]);
    LOG_ERROR(error_log.data());
		return shader_id;
	}
  return shader_id;
}

void OpenGlProgram::use() {
  assert(is_program_created_);
  OpenGlState::getInstance().useProgram(program_id_);
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
