#pragma once

#include <glm/glm.hpp>

#include "Shader.hpp"

class Camera {
  GLuint u_camera = 0;
public:
  glm::mat4 rotate;
  Camera();
  ~Camera();
  operator GLuint() const;
  void Rotate(float x, float y, float z, float deg);
  void attach_to_shader(ShaderProgram &program, const char *symbol);
  void update_uniform();
};
