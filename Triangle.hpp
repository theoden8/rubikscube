#pragma once

#include <cassert>

#include <glm/glm.hpp>

#include "incgraphics.h"
#include "VertexBuffer.hpp"

class Triangle {
  GLuint vao = 0;
  PositionBuffer vertices;
public:
  ColorBuffer *color = NULL;
private:
  void bind_vertex_buffer();
  void create_buffer_layout();
  void enable_vao_attribs();
  void disable_vao_attribs();
public:
  Triangle(PositionBuffer &&vertices, ColorBuffer *colorbuf);
  operator GLuint();
  void init(), update(), draw(), clear();
  ~Triangle();
};
