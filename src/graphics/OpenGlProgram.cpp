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

  storeAllAttributes();
  storeAllUniforms();
  
  is_program_created_ = true;
  return true;
}

void OpenGlProgram::storeAllAttributes() {
  GLint count;

  GLint size; // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 100; // maximum name length
  GLchar name[bufSize]; // variable name in GLSL
  GLsizei length; // name length

  glGetProgramiv(program_id_, GL_ACTIVE_ATTRIBUTES, &count);
  for (GLint i = 0; i < count; i++) {
    glGetActiveAttrib(program_id_, (GLuint)i, bufSize, &length, &size, &type, name);
    attribute_locations_[name] = glGetAttribLocation(program_id_, name);
    LOG_INFO("Attribute: " << name);
  }
}

void OpenGlProgram::storeAllUniforms() {
  GLint count;

  GLint size; // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 100; // maximum name length
  GLchar name[bufSize]; // variable name in GLSL
  GLsizei length; // name length

  glGetProgramiv(program_id_, GL_ACTIVE_UNIFORMS, &count);

  for (GLint i = 0; i < count; i++) {
    glGetActiveUniform(program_id_, (GLuint)i, bufSize, &length, &size, &type, name);
    uniform_locations_[name] = glGetUniformLocation(program_id_, name);
    LOG_INFO("Uniform: " << name);
  }
}

GLint OpenGlProgram::getAttribLocation(const std::string& name) const {
  return attribute_locations_.at(name);
}

bool OpenGlProgram::hasUniform(const std::string& name) const {
  return uniform_locations_.count(name) != 0;
}


void OpenGlProgram::setUniformMatrix4fv(const std::string& name, const glm::mat4& value) {
  assert(hasUniform(name));
  glUniformMatrix4fv(uniform_locations_.at(name), 1, GL_FALSE, glm::value_ptr(value));
}

void OpenGlProgram::setUniform3fv(const std::string& name, const glm::vec3& value) {
  assert(hasUniform(name));
  glUniform3fv(uniform_locations_.at(name), 1, glm::value_ptr(value));
}

void OpenGlProgram::setUniform4fv(const std::string& name, const glm::vec4& value) {
  assert(hasUniform(name));
  glUniform4fv(uniform_locations_.at(name), 1, glm::value_ptr(value));
}

void OpenGlProgram::setUniform1i(const std::string& name, int value) {
  assert(hasUniform(name));
  glUniform1i(uniform_locations_.at(name), value);
}

void OpenGlProgram::setUniformMatrix3fv(const std::string& name, const glm::mat3& value) {
  assert(hasUniform(name));
  glUniformMatrix3fv(uniform_locations_.at(name), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint OpenGlProgram::loadShader(const std::string& source, GLenum type) {
  GLint shader_compiled;
  GLuint shader_id = glCreateShader(type);
  std::string new_source = source;
#ifdef __EMSCRIPTEN__
  new_source = std::string("precision mediump float;");
  new_source += source;

#endif
  const char* c_str = new_source.c_str();
  glShaderSource(shader_id, 1, &c_str, nullptr);
  glCompileShader(shader_id);
  shader_compiled = GL_FALSE;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

  // Get any messages after compilation
  GLint max_length = 0;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);
  if (max_length > 0) {
    std::vector<GLchar> compilation_log(max_length);  // The maxLength includes the NULL character
    glGetShaderInfoLog(shader_id, max_length, &max_length, &compilation_log[0]);
    LOG_INFO(compilation_log.data());
    LOG_DEBUG("Source: " << source);
  }

  if (!shader_compiled) {
    LOG_ERROR("Unable to compile shader type: " << type << ", id: " << shader_id);
  }
  return shader_id;
}

void OpenGlProgram::use() {
  assert(is_program_created_);
  OpenGlState::getInstance().useProgram(program_id_);
}