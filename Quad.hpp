#pragma once

#include <array>

#include <glm/glm.hpp>

#include "Colors.hpp"
#include "VertexBuffer.hpp"
#include "Triangle.hpp"

struct Quad {
  std::array <glm::vec3, 4> corners;
  glm::vec3 color;
  Triangle trifirst, trisecond;

  Quad(std::array <glm::vec3, 4> corners, glm::vec3 color);
  void init(), draw(), clear();
  ~Quad();
};
