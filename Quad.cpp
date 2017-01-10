#include <array>

#include "Quad.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

Quad::Quad(std::array <glm::vec3, 4> corners, glm::vec3 color):
  corners(corners), color(color),
  trifirst(
    PositionBuffer({
      corners[0],
      corners[1],
      corners[2]
    }), color),
  trisecond(
    PositionBuffer({
      corners[0],
      corners[2],
      corners[3]
    }), color)
{}

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

