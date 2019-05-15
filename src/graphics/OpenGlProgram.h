#pragma once

#include <unordered_map>
#include <unordered_set>

#include "graphics/OpenGlResources.h"


class OpenGlProgram {
public:
  OpenGlProgram() = default;
  ~OpenGlProgram();
  bool load(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
  void use();
  void setUniform1i(const std::string& name, int value);
  void setUniformMatrix4fv(const std::string& name, const glm::mat4& value);
  void setUniform3fv(const std::string& name, const glm::vec3& value);
  bool hasUniform(const std::string& name) const;
  GLint getAttribLocation(const std::string& name);

private:
  void getAndStoreUniformLocation(const std::string& name);
  GLuint loadShader(const std::string& source, GLenum type);
  GLint program_id_;
  bool is_program_created_ = false;
  std::unordered_map<std::string, GLint> uniform_locations_;
  mutable std::unordered_map<std::string, bool> uniform_exists_;
};