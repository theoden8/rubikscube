#include "Camera.hpp"

#include <glm/glm.hpp>

#include "Log.hpp"

Camera::Camera():
  rotation(0.f, 0.f, 0.f, 0.f)
{}

Camera::~Camera()
{}

Camera::operator GLuint() const {
  return u_rotation;
}

void Camera::attach_to_shader(ShaderProgram &program, const char *symbol) {
  u_rotation = glGetUniformLocation(program, symbol); GLERROR
}

void Camera::update_uniform() {
  glUniform4f(u_rotation, rotation.x, rotation.y, rotation.z, rotation.a); GLERROR
}
