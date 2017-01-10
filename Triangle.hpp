#pragma once

#include <cassert>

#include <glm/glm.hpp>

#include "incgraphics.h"
#include "VertexBuffer.hpp"

class Triangle {
  GLuint vao = 0;
  PositionBuffer vertices;
  ColorBuffer color;

  void bind_vertex_buffer();
  void create_buffer_layout();
  void enable_vao_attribs();
  void disable_vao_attribs();
public:
  Triangle(PositionBuffer vertices, glm::vec3 &color);
  operator GLuint();
  void init(), draw(), clear();
  ~Triangle();
};
