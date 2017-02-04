#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "incgraphics.h"

struct VertexBuffer {
  GLuint vbo = 0;
  size_t size = 0;
  GLfloat *buffer = NULL;
  bool alloc = false;

  template <typename T = glm::vec3>
    VertexBuffer(std::vector <T> &&data),
    VertexBuffer(std::vector <const T *> &&data);
  operator GLuint();
  void
    init(),
    clear(),
    print() const;
  ~VertexBuffer();
};

typedef VertexBuffer PositionBuffer;
typedef VertexBuffer ColorBuffer;
