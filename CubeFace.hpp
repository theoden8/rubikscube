#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "Quad.hpp"

typedef enum {
  CB_FRONT, CB_BACK,
  CB_LEFT, CB_RIGHT,
  CB_TOP, CB_BOTTOM,
  CB_NO_SIDES
} CB_SIDE;

template <size_t N>
class CubeFace {
  std::vector <CB_SIDE> colors;
  std::array <glm::vec3 *, 4> corners;
public:
  Quad *quads = NULL;
  static glm::vec3 &get_color(CB_SIDE side);
  CubeFace(std::array <glm::vec3 *, 4> corners, CB_SIDE side);
  void construct();
  void init(), draw(), clear();
  ~CubeFace();
};
