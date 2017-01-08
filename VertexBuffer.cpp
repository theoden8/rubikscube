#include <cassert>
#include <stdlib.h>
#include <iostream>
#include <vector>

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

void VertexBuffer::clear() {
  free(buffer);
}

VertexBuffer::~VertexBuffer()
{}
