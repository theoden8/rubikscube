#pragma once

#include <glm/glm.hpp>

#include "CubeFace.hpp"

template <size_t N>
class Cube {
  typedef CubeFace<N> cbface_t;
  cbface_t *sides;
public:
  glm::vec3 center;
  Cube(glm::vec3 center, float h);
  void
    init(),
    draw(),
    clear();
  ~Cube();
};
