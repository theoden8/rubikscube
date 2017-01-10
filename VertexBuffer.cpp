#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "Log.hpp"
#include "VertexBuffer.hpp"

template <>
PositionBuffer::VertexBuffer(std::vector <glm::vec3> data):
  size(3 * data.size())
{
  buffer = (float *)malloc(sizeof(float) * size);
  size_t i = 0;
  for(const auto &c : data) {
    memcpy(&buffer[i], &c.x, sizeof(float) * 3);
    i += 3;
  }
}

VertexBuffer::operator GLuint() {
  return vbo;
}

void VertexBuffer::init() {
  glGenBuffers(1, &vbo); GLERROR
  glBindBuffer(GL_ARRAY_BUFFER, vbo); GLERROR
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), buffer, GL_STATIC_DRAW); GLERROR
  ASSERT(vbo != 0);
}

void VertexBuffer::clear() {
  glDeleteBuffers(1, &vbo); GLERROR
  free(buffer);
}

VertexBuffer::~VertexBuffer()
{}
