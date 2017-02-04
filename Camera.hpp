#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"

class Camera {
  GLuint u_rotation = 0;
public:
  glm::vec4 rotation;
  Camera();
  ~Camera();
  operator GLuint() const;
  void attach_to_shader(ShaderProgram &program, const char *symbol);
  void update_uniform();
};
