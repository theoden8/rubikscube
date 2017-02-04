#include <array>

#include <glm/gtc/type_ptr.hpp>

#include "Quad.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

Quad::Quad(std::array <glm::vec3, 4> &&corners, glm::vec3 &color):
  Quad({&corners[0], &corners[1], &corners[2], &corners[3]}, color)
{}

Quad::Quad(std::array <glm::vec3 *, 4> &&corners, glm::vec3 &color):
  colorbuf(color::make_buffer(&color)),
  trifirst(
    PositionBuffer({
      corners[0],
      corners[1],
      corners[2]
    }), &colorbuf),
  trisecond(
    PositionBuffer({
      corners[0],
      corners[2],
      corners[3]
    }), &colorbuf)
{
  ASSERT(colorbuf.size == 9)
}

void Quad::change_color(glm::vec3 &color) {
  int i = 0;
  for(auto &c : {color, color, color}) {
    memcpy(&(colorbuf.buffer[i]), glm::value_ptr(c), sizeof(float) * 3);
    i += 3;
  }
  update();
}

void Quad::init() {
  // wtf, TODO FIX
  trifirst.color = &colorbuf;
  trisecond.color = &colorbuf;
  ASSERT(trifirst.color == &colorbuf);
  ASSERT(trisecond.color == &colorbuf);
  ASSERT(colorbuf.size == 9);
  colorbuf.init();
  trifirst.init();
  trisecond.init();
}

void Quad::update() {
  trifirst.update();
  trisecond.update();
}

void Quad::draw() {
  trifirst.draw();
  trisecond.draw();
}

void Quad::clear() {
  colorbuf.clear();
  trifirst.clear();
  trisecond.clear();
}

Quad::~Quad()
{}
