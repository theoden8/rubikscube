#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "incgraphics.h"

struct VertexBuffer {
  GLuint vbo = 0;
  size_t size = 0;
  float *buffer = NULL;

  template <typename T = glm::vec3>
  VertexBuffer(std::vector <T> data);
  operator GLuint();
  void
    init(),
    clear();
  ~VertexBuffer();
};

typedef VertexBuffer PositionBuffer;
typedef VertexBuffer ColorBuffer;
