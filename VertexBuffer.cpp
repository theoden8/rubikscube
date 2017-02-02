#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.hpp"
#include "VertexBuffer.hpp"

template <>
PositionBuffer::VertexBuffer(std::vector <glm::vec3> data):
  size(3 * data.size())
{
  buffer = (float *)malloc(sizeof(float) * size);
  size_t i = 0;
  for(const auto &c : data) {
    memcpy(&buffer[i], glm::value_ptr(c), sizeof(float) * 3);
    i += 3;
  }
}

template <>
PositionBuffer::VertexBuffer(std::vector <const glm::vec3 *> data):
  size(3 * data.size())
{
  alloc = true;
  buffer = (GLfloat *)malloc(sizeof(GLfloat) * size);
  assert(buffer != NULL);
  size_t i = 0;
  for(const auto &c : data) {
    memcpy(&buffer[i], glm::value_ptr(*c), sizeof(GLfloat) * 3);
    i += 3;
  }
}

VertexBuffer::operator GLuint() {
  return vbo;
}

void VertexBuffer::init() {
  ASSERT(vbo == 0);
  glGenBuffers(1, &vbo); GLERROR
  glBindBuffer(GL_ARRAY_BUFFER, vbo); GLERROR
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(GLfloat), buffer, GL_STATIC_DRAW); GLERROR
  ASSERT(vbo != 0);
}

void VertexBuffer::clear() {
  glDeleteBuffers(1, &vbo); GLERROR
  alloc = false;
  ASSERT(!alloc || buffer != NULL);
  if(alloc)
    free(buffer);
}

void VertexBuffer::print() const {
  for(int i = 0; i < size; ++i) {
    if(i)
      printf(", ");
    printf("%f", buffer[i]);
  }
  putchar('\n');
}

VertexBuffer::~VertexBuffer()
{}
