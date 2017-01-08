#include <cassert>

#include "Triangle.hpp"
#include "Log.hpp"
#include "Colors.hpp"

void Triangle::bind_vertex_buffer() {
  glGenBuffers(1, &vertices.vbo); GLERROR
  glBindBuffer(GL_ARRAY_BUFFER, vertices); GLERROR
  glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(float), vertices.buffer, GL_STATIC_DRAW); GLERROR
  assert(vertices.vbo != 0);

  glGenBuffers(1, &color.vbo); GLERROR
  glBindBuffer(GL_ARRAY_BUFFER, color); GLERROR
  glBufferData(GL_ARRAY_BUFFER, color.size * sizeof(float), color.buffer, GL_STATIC_DRAW); GLERROR
  assert(color.vbo != 0);
}

void Triangle::create_buffer_layout() {
  glGenVertexArrays(1, &vao); GLERROR
}

void Triangle::enable_vao_attribs() {
  glBindVertexArray(vao); GLERROR

  glBindBuffer(GL_ARRAY_BUFFER, vertices.vbo); GLERROR
  glEnableVertexAttribArray(0); GLERROR // layout(location == 0)
  glVertexAttribPointer(0, vertices.size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR

  glBindBuffer(GL_ARRAY_BUFFER, color.vbo); GLERROR
  glEnableVertexAttribArray(1); GLERROR // layout(location == 1)
  glVertexAttribPointer(1, color.size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR
}

void Triangle::disable_vao_attribs() {
  glDisableVertexAttribArray(0); GLERROR
  glDisableVertexAttribArray(1); GLERROR
}


Triangle::Triangle(PositionBuffer vertices, glm::vec3 &color):
  vertices(vertices), color(make_color_buffer(color))
{
  ASSERT(vertices.size == 3 * 3);
}

void Triangle::init() {
  bind_vertex_buffer();
  create_buffer_layout();
  enable_vao_attribs();
}

void Triangle::draw() {
  glBindVertexArray(vao); GLERROR
  glDrawArrays(GL_TRIANGLES, 0, 3); GLERROR
}

Triangle::operator GLuint() {
  return vao;
}

void Triangle::clear() {
  glDeleteBuffers(1, &vertices.vbo); GLERROR
  glDeleteBuffers(1, &color.vbo); GLERROR
  glDeleteVertexArrays(1, &vao); GLERROR
}

Triangle::~Triangle()
{
  vertices.clear();
  color.clear();
}
