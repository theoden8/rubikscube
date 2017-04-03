#include <cassert>

#include "Triangle.hpp"
#include "Quad.hpp"
#include "Log.hpp"
#include "Colors.hpp"
#include "Window.hpp"

void Triangle::bind_vertex_buffer() {
  vertices.init();
  ASSERT(vertices.vbo != 0);
}

void Triangle::create_buffer_layout() {
  glGenVertexArrays(1, &vao); GLERROR
}

void Triangle::enable_vao_attribs() {
  glBindVertexArray(vao); GLERROR

  ASSERT(vertices.size == 3 * 3);
  /* vertices.print(); */
  glBindBuffer(GL_ARRAY_BUFFER, vertices.vbo); GLERROR
  glEnableVertexAttribArray(0); GLERROR // layout(location == 0)
  glVertexAttribPointer(0, vertices.size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR

  ASSERT(color->size == 3 * 3);
  /* color->print(); */
  glBindBuffer(GL_ARRAY_BUFFER, color->vbo); GLERROR
  glEnableVertexAttribArray(1); GLERROR // layout(location == 1)
  glVertexAttribPointer(1, color->size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR
}

void Triangle::disable_vao_attribs() {
  glDisableVertexAttribArray(0); GLERROR
  glDisableVertexAttribArray(1); GLERROR
}


Triangle::Triangle(PositionBuffer &&vertices, ColorBuffer *colorbuf):
  vertices(vertices), color(colorbuf)
{
  ASSERT(vertices.size == 3 * 3);
  ASSERT(colorbuf->size == 3 * 3);
  ASSERT(color->size == 3 * 3);
  /* colorbuf->print(); */
}

Triangle::operator GLuint() {
  return vao;
}

void Triangle::init() {
  ASSERT(color->size == 9);
  bind_vertex_buffer();
  create_buffer_layout();
  enable_vao_attribs();
}

void Triangle::update() {
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vertices);
  glVertexAttribPointer(0, vertices.size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR

  glBindBuffer(GL_ARRAY_BUFFER, *color);
  glVertexAttribPointer(1, color->size / 3, GL_FLOAT, GL_FALSE, 0, NULL); GLERROR
}

void Triangle::draw() {
  ASSERT(vao != 0);
  glBindVertexArray(vao); GLERROR
  glDrawArrays(GL_TRIANGLES, 0, 3); GLERROR
}

void Triangle::clear() {
  vertices.clear();
  glDeleteVertexArrays(1, &vao); GLERROR
}

Triangle::~Triangle()
{}
