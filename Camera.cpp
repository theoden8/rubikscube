#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.hpp"

Camera::Camera()
{}

Camera::~Camera()
{}

Camera::operator GLuint() const {
  return u_camera;
}

void Camera::Rotate(float x, float y, float z, float deg) {
  rotate = glm::rotate(glm::radians(deg), glm::vec3(x, y, z)) * rotate;
}

void Camera::attach_to_shader(ShaderProgram &program, const char *symbol) {
  u_camera = glGetUniformLocation(program, symbol); GLERROR
}

void Camera::update_uniform() {
  glUniformMatrix4fvARB(u_camera, 1 , GL_FALSE, glm::value_ptr(rotate)); GLERROR
}
