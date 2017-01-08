#ifndef CUBEFACE_CPP_ZPIJBQXO
#define CUBEFACE_CPP_ZPIJBQXO

#include "Quad.hpp"
#include "VertexBuffer.hpp"
#include "Log.hpp"
#include "Colors.hpp"

#include <array>
#include <vector>

template <size_t N>
class CubeFace {
  std::array <int, N * N> colors;
  std::array <Quad, N * N> quads;
  const Quad corners;
public:
  static ColorBuffer get_color(int color) {
    static glm::vec3 palette[6] = {
      color::black, color::yellow, color::red,
      color::green, color::blue, color::teal
    };
    return make_color_buffer(palette[color]);
  }

  CubeFace(int color, Quad &corners):
    colors(color), quads(NULL), corners(corners)
  {}

  void init() {
  }
};

#endif
