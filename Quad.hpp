#pragma once

#include <glm/glm.hpp>

#include "VertexBuffer.hpp"
#include "Triangle.hpp"

struct Quad {
  PositionBuffer position;
  glm::vec3 color;
  Triangle trifirst, trisecond;

  Quad(PositionBuffer position, glm::vec3 &color);
  void init();
  void draw();
  void clear();
  ~Quad();
};
