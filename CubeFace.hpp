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
} SIDE;

template <size_t N>
class CubeFace {
  std::vector <SIDE> colors;
  Quad *quads;
public:
  static glm::vec3 get_color(SIDE side);
  CubeFace(std::array <glm::vec3, 4> corners, SIDE side);
  void init(), draw(), clear();
  ~CubeFace();
};
