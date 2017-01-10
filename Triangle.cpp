#include <cassert>

#include "Triangle.hpp"
#include "Log.hpp"
#include "Colors.hpp"

void Triangle::bind_vertex_buffer() {
  vertices.init();
  color.init();
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

Triangle::operator GLuint() {
  return vao;
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

void Triangle::clear() {
  vertices.clear();
  color.clear();
  glDeleteVertexArrays(1, &vao); GLERROR
}

Triangle::~Triangle()
{}
