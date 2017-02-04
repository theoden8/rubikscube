#pragma once

#include <glm/glm.hpp>

#include "CubeFace.hpp"

template <size_t N>
class Cube {
  typedef CubeFace<N> cbface_t;
  cbface_t *sides;
public:
  typedef enum { CB_RT_CW, CB_RT_CCW } CB_CW_CCW;
  glm::vec3 center;
  Cube(glm::vec3 &&center, float h);
  void
    rotate(CB_SIDE, CB_CW_CCW),
    init(),
    draw(),
    clear();
  ~Cube();
};
