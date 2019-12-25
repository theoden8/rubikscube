#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Logger.hpp>

#include <Transformation.hpp>

class Camera {
public:
  Transformation transform;
  bool &has_changed;

  Camera():
    has_changed(transform.has_changed)
  {}

  glm::mat4 get_matrix() {
    return transform.get_matrix();;
  }
};
