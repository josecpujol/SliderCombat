#include "OpenGlProgram.h"

#include "utils/Logger.h"
#include "graphics/OpenGlState.h"

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

GLint OpenGlProgram::getAttribLocation(const std::string& name) {
  GLint location = glGetAttribLocation(program_id_, name.c_str());
  OpenGlResources::checkGlError();
  return location;
}

void OpenGlProgram::getAndStoreUniformLocation(const std::string& name) {
  if (uniform_locations_.count(name) == 0) {
    GLint location = glGetUniformLocation(program_id_, name.c_str());
    OpenGlResources::checkGlError();
    assert(location != -1);
    uniform_locations_[name] = location;
  }
}

void OpenGlProgram::setUniformMatrix4fv(const std::string& name, const glm::mat4& value) {
  getAndStoreUniformLocation(name);
  glUniformMatrix4fv(uniform_locations_[name], 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGlProgram::setUniform1i(const std::string& name, int value) {
  getAndStoreUniformLocation(name);
  glUniform1i(uniform_locations_[name], value);
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