#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "incgraphics.h"

struct VertexBuffer {
  GLuint vbo = 0;
  const size_t size;
  float *buffer = NULL;

  template <typename T = glm::vec3>
  VertexBuffer(std::vector <T> data);
  operator GLuint();
  void clear();
  ~VertexBuffer();
};

typedef VertexBuffer PositionBuffer;
typedef VertexBuffer ColorBuffer;
