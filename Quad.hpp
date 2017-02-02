#pragma once

#include <array>

#include <glm/glm.hpp>

#include "Colors.hpp"
#include "VertexBuffer.hpp"
#include "Triangle.hpp"

struct Quad {
  ColorBuffer colorbuf;
  Triangle trifirst, trisecond;

  Quad(std::array <glm::vec3, 4> corners, glm::vec3 &color);
  Quad(std::array <glm::vec3 *, 4> corners, glm::vec3 &color);
  void change_color(glm::vec3 &color);
  void init(), update(), draw(), clear();
  ~Quad();
};
