#include "Quad.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

#define p position.buffer
Quad::Quad(PositionBuffer position, glm::vec3 &color):
  color(color),
  position(position),
  trifirst(
    PositionBuffer({
      glm::vec3(p[0], p[1], p[2]),
      glm::vec3(p[3], p[4], p[5]),
      glm::vec3(p[6], p[7], p[8]),
    }), color),
  trisecond(
    PositionBuffer({
      glm::vec3(p[0], p[1], p[2]),
      glm::vec3(p[6], p[7], p[8]),
      glm::vec3(p[9], p[10], p[11]),
    }), color)
#undef p
{
  ASSERT(position.size == 3 * 4);
}

void Quad::init() {
  trifirst.init();
  trisecond.init();
}

void Quad::draw() {
  trifirst.draw();
  trisecond.draw();
}

void Quad::clear() {
  trifirst.clear();
  trisecond.clear();
}

Quad::~Quad()
{}

