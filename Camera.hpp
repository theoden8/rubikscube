#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Logger.hpp>

#include <Transformation.hpp>

class Camera {
  Transformation transform;

public:
  Camera()
  {}

  void rotate(float x, float y, float z, float deg) {
    transform.Rotate(x, y, z, deg);
  }

  bool has_changed() {
    return transform.has_changed;
  }

  glm::mat4 get_matrix() {
    return transform.get_matrix();;
  }
};
